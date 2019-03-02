#pragma once

#include <cstdint>
#include <string>
#include <sstream>

#include <vector>
#include <array>

namespace minilockcpp
{

class utils
{
public:
    void print_vector(std::vector<uint8_t>& v, const std::string& info);
    std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    std::string base64_decode(std::string const& encoded_string);
    bool base58_Decode(void *bin, size_t *binszp, const char *b58, size_t b58sz);
    bool base58_Encode(char *b58, size_t *b58sz, const void *data, size_t binsz);

protected:
    void open_file(std::fstream& file, const std::ios_base::openmode& flags, const std::string& filename, const std::string& working_directory);
    void close_file(std::fstream& file);
    void remove_file(const std::string& , const std::string&);

    static void incrementChunkCounter(uint8_t *fullNonce);
    static int readChunkLength(uint8_t *data, size_t szData, size_t offset);
    static int writeChunkLength(uint8_t *data, size_t szData, size_t chunkLen);
    static void setLastChunkFlag(uint8_t *fullNonce);
};

}

