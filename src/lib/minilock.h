/*
 * ToDo: - base64_encode header size_t swap
 *       - pass vector of strings (recipients)
 *       - sequence multiple files
 *       - test multiple ids
 *
 */

#pragma once

#include "header.h"
#include "encryption.h"
#include "decryption.h"
//#include "utils.h"
#include "constants.h"
#include "logging.h"

#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>

namespace minilockcpp
{
    class minilock : public encryption, public decryption
    {
    public:
        minilock();
        ~minilock();
        minilock(const std::string& email, const std::string& password);

        void set_log_level(log::log_level level);

        bool initialize(const std::string& email, const std::string& password);

        bool encrypt_file(const std::string& source_filename,
                          bool random_filename = false,
                          const std::string& source_directory={},
                          const std::string& target_directory={},
                          const std::vector<std::string>& recipients = std::vector<std::string>(),
                          bool is_own_recipient = true);

        bool decrypt_file(const std::string& source_filename,
                          const std::string& source_directory={},
                          const std::string& target_directory={});

        std::string get_id() const { return this->id; }
        std::string get_email() const { return this->email; }

    private:
        void check_initialization();

    private:
        log::log_level level = log::log_level::verbose;
        bool initialized = false;
        std::string id;
        std::string email;
        std::array<uint8_t, minilock_constant::publickey_bytes> publickey;
        std::array<uint8_t, minilock_constant::secretkey_bytes> secretkey;
    };
}
