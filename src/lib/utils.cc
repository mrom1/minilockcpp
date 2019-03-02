#include "utils.h"
#include "exception.h"
#include "constants.h"
#include "logging.h"

#include <fstream>
#include <experimental/filesystem>
#include <string.h>
#include <stdlib.h>

namespace minilockcpp
{

const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

void utils::open_file(std::fstream& file, const std::ios_base::openmode& flags, const std::string& filename, const std::string& working_directory)
{
    auto f = std::experimental::filesystem::absolute(filename, working_directory);
    if(!std::experimental::filesystem::exists(f) &&  !(flags & (std::ios::out)) )
    {
        log::error() << "trying to open file " << filename << ": could not find file " << f << std::endl;
        throw file_not_found_error() << "File not found error: Unable to find file " << f;
    }
    file.open(f.c_str(), flags);
    if (!file.rdbuf()->is_open())
    {
        log::error() << "could not open file " << f << std::endl;
        throw io_error() << "IO Error: unable to open file " << f;
    }

    log::debug() << "opened file " << f << std::endl;
}

void utils::remove_file(const std::string& filename, const std::string& working_directory)
{
    auto f = std::experimental::filesystem::absolute(filename, working_directory);
    if(!std::experimental::filesystem::exists(f))
    {
        log::error() << "trying to remove file " << filename << ": could not find file " << f << std::endl;
        throw file_not_found_error() << "File not found error: Unable to find file " << f;
    }
    if(std::remove(f.c_str()) != 0)
    {
        log::warning() << "could not delete file " << filename << " in directory " << working_directory << std::endl;
    }
    else
    {
        log::debug() << "removed file " << f << std::endl;
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

std::string utils::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{

    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;

}

std::string utils::base64_decode(std::string const& encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    return ret;
}


}
