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


#ifndef _MINILOCKCPP_MINILOCK_H_
#define _MINILOCKCPP_MINILOCK_H_


#include "header.h"
#include "encryption.h"
#include "decryption.h"
#include "constants.h"
#include "logging.h"

#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>


namespace minilockcpp
{
/**
 * @brief Class minilock.
 * 
 *        This is the main class of the library.
 *        Use this class to start a session and decrypt or encrypt files.
 * 
 *        To open a minilock session do this:
 *        auto session = new minilockcpp::minilock("myemail@address", "mypassword");
 * 
 *        To encrypt a file call it like this:
 *        bool result = session->encrypt_file("myfile.doc", ...);
 *
 *        To decrypt a file call it like this:
 *        bool result = session->decrypt_file("myfile.doc", ...);
 * 
 */
class minilock : public encryption, public decryption
{
public:
    /**
     * @brief Default constructor.
    */
    minilock();


    /**
     * @brief This constructor will immediately initialize a session.
     * @param email E-Mail address of the user.
     * @param password Password of the user.
    */
    minilock(const std::string& email, const std::string& password);


    /**
     * @brief Deconstructor. Closing the session.
    */
    ~minilock();


    /**
     * @brief Setting the log level. See log class in logging.h for more information.
     * @param level Log level (None -> Error -> Warning -> Info -> Verbose -> Debug).
    */
    void set_log_level(log::log_level level);


    /**
     * @brief Initialize the session for one user.
     * @param email E-Mail address of the user.
     * @param password Password of the user.
     * @return returns true on success, otherwise false.
    */
    bool initialize(const std::string& email, const std::string& password);


    /**
     * @brief Encrypt a given file.
     * @param source_filename name of the file to be encrypted without a leading path.
     * @param random_filename if set the encrypted file will have a random name.
     * @param source_directory path of the directory which cointains the file to be encrypted.
     * @param target_directory path of the directory in which the encrypted file will be written to.
     * @param recipients list of recipients
     * @param is_own_recipient If true the owner of the file will be added as a recipient. 
     * @param return returns true on success, otherwise false.
    */
    bool encrypt_file(const std::string& source_filename,
                      bool random_filename = false,
                      const std::string& source_directory={},
                      const std::string& target_directory={},
                      const std::vector<std::string>& recipients = std::vector<std::string>(),
                      bool is_own_recipient = true);


    /**
     * @brief Decryption of a file
     * @param source_filename filename string without path.
     * @param source_directory path of the directory for the file to be decrypted.
     * @param target_directory path where the decrypted file will be saved to.    
     * @param return returns true on success, otherwise false.
     */
    bool decrypt_file(const std::string& source_filename,
                      const std::string& source_directory={},
                      const std::string& target_directory={});


    /**
     * @brief Retrun generated ID of the user in this session.
     * @return generated ID to be shared.
    */
    std::string get_id() const;


    /**
     * @brief Return E-Mail address of the user in this session.
     * @return E-Mail address of the user logged in.
    */
    std::string get_email() const;


private:
    /**
     * @brief Throws initialize_error exception if uninitialized.
    */
    void check_initialization();


private:
    log::log_level level = log::log_level::info;
    bool initialized = false;
    std::string id;
    std::string email;
    std::array<uint8_t, minilock_constant::publickey_bytes> publickey;
    std::array<uint8_t, minilock_constant::secretkey_bytes> secretkey;
};

} // namespace minilockcpp


#endif // _MINILOCKCPP_MINILOCK_H_