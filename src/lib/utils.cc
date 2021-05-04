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



#include "utils.h"
#include "exception.h"
#include "constants.h"
#include "logging.h"

#include <fstream>
#include <filesystem>
#include <string.h>
#include <stdlib.h>

using namespace minilockcpp;


void utils::open_file(std::fstream& file, const std::ios_base::openmode& flags, const std::string& filename, const std::string& working_directory)
{
    std::filesystem::path filepath;

    if (working_directory.empty())
    {
        filepath = std::filesystem::absolute(std::filesystem::path(std::filesystem::current_path().string() + "/" + filename));
    }
    else
    {
        filepath = std::filesystem::absolute(std::filesystem::path(working_directory + "/" + filename));
    }

    if(!std::filesystem::exists(filepath) &&  !(flags & (std::ios::out)) )
    {
        log::error() << "trying to open file " << filename << ": could not find file " << filepath << std::endl;
        throw file_not_found_error() << "File not found error: Unable to find file " << filepath;
    }

    file.open(filepath.c_str(), flags);
    if (!file.rdbuf()->is_open())
    {
        log::error() << "could not open file " << filepath << std::endl;
        throw io_error() << "IO Error: unable to open file " << filepath;
    }

    log::debug() << "opened file " << filepath << std::endl;
}

void utils::remove_file(const std::string& filename, const std::string& working_directory)
{
    std::filesystem::path filepath;

    if (working_directory.empty())
    {
        filepath = std::filesystem::path(std::filesystem::current_path().string() + "/" + filename);
    }
    else
    {
        filepath = std::filesystem::absolute(std::filesystem::path(working_directory + "/" + filename));
    }

    if(!std::filesystem::remove(filepath))
    {
        log::warning() << "could not delete file " << filename << " in directory " << working_directory << std::endl;
        throw file_not_found_error() << "File not found error: Unable to find file " << filepath;
    }
    else
    {
        log::debug() << "removed file " << filepath << std::endl;
    }
}

void utils::close_file(std::fstream& file)
{
    file.close();
    if(file.rdbuf()->is_open())
    {
        log::warning() << "unable to close file " << std::endl;
        throw io_error() << "IO Error: unable to close file";
    }
}

void utils::incrementChunkCounter(uint8_t *fullNonce) {
    for (size_t i = minilock_constant::filenonce_bytes; i < minilock_constant::fullnonce_bytes; i++) {
        fullNonce[i]++;
        if (fullNonce[i]) break;
    }
}

int utils::readChunkLength(uint8_t *data, size_t szData, size_t offset) {
    int ret;
    offset |= 0;

    if (szData < offset + 4) return -1;

    ret = ( (data[offset+3] << 24L ) |
            (data[offset+2] << 16L ) |
            (data[offset+1] <<  8L ) |
            (data[offset+0] <<  0L ) );

    return ret;
}

int utils::writeChunkLength(uint8_t *data, size_t szData, size_t chunkLen) {
    if(szData != minilock_constant::chunk_array_length) return 0;
    else
    {
        data[0] = (chunkLen >>  0) & 0xff;
        data[1] = (chunkLen >>  8) & 0xff;
        data[2] = (chunkLen >> 16) & 0xff;
        data[3] = (chunkLen >> 24) & 0xff;
        return 0;
    }
}

void utils::setLastChunkFlag(uint8_t *fullNonce) {
    fullNonce[minilock_constant::fullnonce_bytes-1] |= 0x80;
}

