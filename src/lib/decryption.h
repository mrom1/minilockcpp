#pragma once

#include "header.h"
#include <mutex>

namespace minilockcpp {
class decryption : public header
{
protected:
    void decrypt(const std::string& source_filename,
                 const uint8_t* const secretkey,
                 const std::string& source_directory={},
                 const std::string& target_directory={});

private:
    void decrypt_stream(std::istream& instream,
                        std::ostream& outstream,
                        uint64_t pos_end = 0);

    std::string find_header(std::istream& is);
    std::string decrypt_filename(std::istream& is);

private:
    std::mutex _mutex;
};
}
