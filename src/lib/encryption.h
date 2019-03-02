#pragma once

#include "header.h"
#include <mutex>

namespace minilockcpp {

class encryption : public header
{
protected:
    void encrypt(const std::string& source_filename,
                 const std::string& id,
                 const uint8_t* const secretkey,
                 bool random_filename = false,
                 const std::string& source_directory={},
                 const std::string& target_directory={},
                 const std::vector<std::string>& recipients = std::vector<std::string>(),
                 bool is_own_recipient = true);

private:
    void encrypt_stream(std::istream& instream,
                        std::ostream& outstream,
                        uint64_t pos_end = 0);

    void stream_header(std::ostream& outstream, const std::string& json_header);
    std::string encrypt_filename(const std::string& filename);
    std::string random_filename();

private:
    std::mutex _mutex;
};

}
