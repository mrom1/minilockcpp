#include "encryption.h"
#include "logging.h"
#include "exception.h"

#include <blake2.h>
#include <sodium.h>

#include <fstream>
#include <random>
#include <algorithm>

namespace minilockcpp {

void encryption::encrypt(const std::string& source_filename,
                         const std::string& id,
                         const uint8_t* const secretkey,
                         bool random_filename,
                         const std::string& source_directory,
                         const std::string& target_directory,
                         const std::vector<std::string>& recipients,
                         bool is_own_recipient)
{
    std::lock_guard<std::mutex> lock(_mutex);

    file_key.fill(0);
    file_nonce.fill(0);
    full_nonce.fill(0);

    std::fstream ifs, ofs;
    std::string target_filename = random_filename ? this->random_filename() : source_filename + ".minilock";

    open_file(ifs, (std::ios::in | std::ios::binary), source_filename, source_directory);

    ifs.seekg(0, std::ios::end);
    size_t filesize = static_cast<std::size_t>(ifs.tellg());
    ifs.seekg(0, std::ios::beg);

    open_file(ofs, (std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary), target_filename, target_directory);

    randombytes_buf(file_nonce.data(), minilock_constant::filekey_bytes);
    randombytes_buf(file_key.data(), minilock_constant::filenonce_bytes);

    std::string encrypted_filename = encrypt_filename(source_filename);
    ofs << encrypted_filename;
    encrypt_stream(ifs, ofs, filesize);
    size_t content_size = static_cast<std::size_t>(ofs.tellg());
    close_file(ifs);

    ofs.seekg(std::ios::beg);
    std::stringstream ssbuffer;
    ssbuffer << ofs.rdbuf();

    std::array<uint8_t, blake2s_constant::BLAKE2S_OUTBYTES> filehash;
    blake2s(filehash.data(), ssbuffer.str().c_str(), nullptr, filehash.size(), content_size, 0);

    std::string json_header = generate_header(id,
                                              filehash.data(),
                                              secretkey,
                                              recipients,
                                              is_own_recipient);
    ofs.seekp(std::ios::beg);
    stream_header(ofs, json_header);
    ofs << ssbuffer.str();

    close_file(ofs);
}

void encryption::encrypt_stream(std::istream &instream, std::ostream &outstream, uint64_t pos_end)
{
    uint64_t chunk_size = minilock_constant::chunk_size;
    bool last_chunk = false;

    while(!instream.eof())
    {
        std::array<uint8_t, minilock_constant::chunk_array_length> array_chunk_length;

        incrementChunkCounter(full_nonce.data());

        if(pos_end)
        {
            uint64_t curr_pos = static_cast<uint32_t>(instream.tellg());

            if(curr_pos > (pos_end-minilock_constant::chunk_size))
            {
                chunk_size = pos_end - curr_pos;
                setLastChunkFlag(full_nonce.data());
                last_chunk = true;
            }
            else if(static_cast<int>((pos_end - minilock_constant::chunk_size)) < 0)
            {
                chunk_size = pos_end;
                setLastChunkFlag(full_nonce.data());
                last_chunk = true;
            }
            else
            {
                chunk_size = minilock_constant::chunk_size;
            }
        }

        std::vector<uint8_t> decrypted_chunk(chunk_size);
        std::vector<uint8_t> encrypted_chunk(chunk_size + minilock_constant::padding_bytes);

        instream.read(reinterpret_cast<char*>(decrypted_chunk.data()), static_cast<long>(decrypted_chunk.size()));

        if(crypto_secretbox_easy(encrypted_chunk.data(), decrypted_chunk.data(), decrypted_chunk.size(), full_nonce.data(), file_key.data()))
        {
            throw general_encryption_error() << "Encryption error at byte position: " << instream.tellg() << " with current chunk size: " << chunk_size;
        }

        writeChunkLength(array_chunk_length.data(), array_chunk_length.size(), chunk_size);
        outstream.write(reinterpret_cast<char*>(array_chunk_length.data()), array_chunk_length.size());
        outstream.write(reinterpret_cast<char*>(encrypted_chunk.data()), static_cast<long>(encrypted_chunk.size()));

        if(last_chunk)
            break;
    }
}

std::string encryption::encrypt_filename(const std::string& filename)
{
    std::array<uint8_t, minilock_constant::chunk_array_length> array_chunk_length;
    std::vector<uint8_t> padded_filename(minilock_constant::filename_padding - filename.size(),0);
    padded_filename.insert(padded_filename.begin(), filename.begin(), filename.end());
    std::vector<uint8_t> encrypted_filename(padded_filename.size() + minilock_constant::padding_bytes);

    std::copy(file_nonce.begin(), file_nonce.end(), full_nonce.data());

    if (0 != crypto_secretbox_easy(encrypted_filename.data(), padded_filename.data(), padded_filename.size(), full_nonce.data(), file_key.data()) )
    {
        throw general_encryption_error() << "Encryption Error: Unable to encrypt the original filename.";
    }

    writeChunkLength(array_chunk_length.data(), array_chunk_length.size(), padded_filename.size());

    return (  std::string(array_chunk_length.data(), array_chunk_length.data() + array_chunk_length.size())
              + std::string(encrypted_filename.data(), encrypted_filename.data() + encrypted_filename.size()));
}

void encryption::stream_header(std::ostream& outstream, const std::string& json_header)
{
    std::array<uint8_t, minilock_constant::chunk_array_length> array_chunk_length;
    writeChunkLength(array_chunk_length.data(), array_chunk_length.size(), json_header.size());
    outstream.write(magic_bytes, minilock_constant::magicbytes_length);
    outstream.write(reinterpret_cast<char*>(array_chunk_length.data()), array_chunk_length.size());
    outstream.write(json_header.c_str(), static_cast<long>(json_header.size()));
}


std::string encryption::random_filename()
{
     std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

     std::random_device rd;
     std::mt19937 generator(rd());

     std::shuffle(str.begin(), str.end(), generator);

     return str.substr(0, 32);
}

}
