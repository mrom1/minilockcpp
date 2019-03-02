#pragma once
#include "utils.h"
#include "constants.h"

#include <string>
#include <array>

namespace minilockcpp
{

class header : public utils
{
protected:
    bool parse_header(const std::string& json_header,
                      const uint8_t* const secretkey);

    std::string generate_header(const std::string& sender_id,
                                const uint8_t* filehash,
                                const uint8_t* secretkey,
                                std::vector<std::string> recipients,
                                bool is_own_recipient=true);

protected:
    std::array<uint8_t, minilock_constant::filekey_bytes> file_key;
    std::array<uint8_t, minilock_constant::filenonce_bytes> file_nonce;
    std::array<uint8_t, minilock_constant::fullnonce_bytes> full_nonce;
};

}
