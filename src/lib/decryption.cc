/************************************************************************
 * This file is part of the minilockcpp distribution
 * (https://github.com/mrom1/minilockcpp).
 * Copyright (c) 2021 mrom1.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/



#include "decryption.h"
#include "logging.h"
#include "exception.h"

#include <sodium.h>

#include <fstream>

using namespace minilockcpp;

void decryption::decrypt(const std::string& source_filename,
                         const uint8_t* const secretkey,
                         const std::string& source_directory,
                         const std::string& target_directory)

{
    std::lock_guard<std::mutex> lock(_mutex);

    std::fstream ifs, ofs;

    open_file(ifs, (std::ios::in | std::ios::binary),
              source_filename, source_directory);

    ifs.seekg(0, std::ios::end);
    size_t filesize = static_cast<size_t>(ifs.tellg());
    ifs.seekg(0, std::ios::beg);

    std::string json_header = find_header(ifs);

    parse_header(json_header, secretkey);

    ifs.seekg(static_cast<long>(
                  minilock_constant::magicbytes_length +
                  minilock_constant::chunk_array_length +
                  json_header.size()),
              std::ios::beg);

    std::string target_filename = decrypt_filename(ifs);
    if(target_filename.empty())
    {
        target_filename = source_filename + ".decrypted";
        log::warning() << "unable to fetch original filename. using filename " << target_filename << std::endl;
    }

    open_file(ofs, (std::ios::out | std::ios::binary),
              target_filename, target_directory);

    decrypt_stream(ifs, ofs, filesize);

    close_file(ifs);
    close_file(ofs);
}

void decryption::decrypt_stream(std::istream& instream, std::ostream& outstream, uint64_t pos_end)
{
    bool last_chunk = false;
    while(!instream.eof())
    {
        std::array<uint8_t, minilock_constant::chunk_array_length> array_chunk_length;
        instream.read(reinterpret_cast<char*>(array_chunk_length.data()), array_chunk_length.size());

        uint32_t chunk_length = static_cast<uint32_t>(readChunkLength(array_chunk_length.data(), array_chunk_length.size(), 0));
        std::vector<uint8_t> encrypted_chunk(chunk_length + minilock_constant::padding_bytes);
        std::vector<uint8_t> decrypted_chunk(chunk_length);

        size_t curr_pos = static_cast<size_t>(instream.tellg());

        incrementChunkCounter(full_nonce.data());

        if(pos_end)
        {
            if(curr_pos + chunk_length + minilock_constant::padding_bytes >= pos_end)
            {
                setLastChunkFlag(full_nonce.data());
                last_chunk = true;
            }
        }

        instream.read(reinterpret_cast<char*>(encrypted_chunk.data()), static_cast<long>(encrypted_chunk.size()));

        if(0 != crypto_secretbox_open_easy(decrypted_chunk.data(), encrypted_chunk.data(), encrypted_chunk.size(), full_nonce.data(), file_key.data()))
        {
            throw general_decryption_error() << "Decryption error on chunk at byte position: " << static_cast<unsigned int>(instream.tellg()) - chunk_length;
        }

        outstream.write(reinterpret_cast<char*>(decrypted_chunk.data()), static_cast<long>(decrypted_chunk.size()));

        if(last_chunk)
            break;
    }
}

std::string decryption::decrypt_filename(std::istream& is)
{
    std::array<uint8_t, minilock_constant::chunk_array_length> array_chunk_length;

    is.read(reinterpret_cast<char*>(array_chunk_length.data()), array_chunk_length.size());

    const uint32_t filename_length = static_cast<uint32_t>(readChunkLength(array_chunk_length.data(), array_chunk_length.size(), 0));
    std::vector<uint8_t> encrypted_filename(filename_length + minilock_constant::padding_bytes);
    std::vector<uint8_t> decrypted_filename(filename_length);

    is.read(reinterpret_cast<char*>(encrypted_filename.data()), static_cast<long>(encrypted_filename.size()));
    if(0 != crypto_secretbox_open_easy(decrypted_filename.data(), encrypted_filename.data(), encrypted_filename.size(), full_nonce.data(), file_key.data()))
    {
        log::warning() << "unable to decrypt original filename" << std::endl;
        return {};
    }

    return std::string(decrypted_filename.data(), decrypted_filename.data()+filename_length);
}

std::string decryption::find_header(std::istream& is)
{
    std::string json_header;
    std::array<uint8_t, minilock_constant::chunk_array_length> array_chunk_length;

    is.seekg(minilock_constant::magicbytes_length, std::ios::beg);
    is.read(reinterpret_cast<char*>(array_chunk_length.data()), array_chunk_length.size());

    const uint32_t headerLength = static_cast<uint32_t>(readChunkLength(array_chunk_length.data(), array_chunk_length.size(), 0));
    is.seekg(minilock_constant::chunk_array_length + minilock_constant::magicbytes_length, std::ios::beg);
    json_header.resize(headerLength);
    is.read(&json_header[0], headerLength);

    return json_header;
}