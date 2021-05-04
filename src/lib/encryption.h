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


#ifndef _MINILOCKCPP_ENCRYPTION_H_
#define _MINILOCKCPP_ENCRYPTION_H_


#include "header.h"
#include <mutex>

namespace minilockcpp 
{
/**
 * @brief Encryption functionallity class.
*/
class encryption : public header
{
protected:
    /**
     * @brief Encrypt a given file.
     * @param source_filename name of the file to be encrypted without a leading path.
     * @param id The ID of the owner which will encrypt the file.
     * @param secretkey pointer to the secret key array.
     * @param random_filename if set the encrypted file will have a random name.
     * @param source_directory path of the directory which cointains the file to be encrypted.
     * @param target_directory path of the directory in which the encrypted file will be written to.
     * @param recipients list of recipients
     * @param is_own_recipient If true the owner of the file will be added as a recipient. 
    */
    void encrypt(const std::string& source_filename,
                 const std::string& id,
                 const uint8_t* const secretkey,
                 bool random_filename = false,
                 const std::string& source_directory={},
                 const std::string& target_directory={},
                 const std::vector<std::string>& recipients = std::vector<std::string>(),
                 bool is_own_recipient = true);


private:
    /**
     * @brief Utility function. Encrypt a filestream.
     * @param instream input stream.
     * @param outstream output stream.
     * @param pos_end ending position. Default starts at '0'.
    */
    void encrypt_stream(std::istream& instream,
                        std::ostream& outstream,
                        uint64_t pos_end = 0);


    /**
     * @brief Generate a encrypted JSON header.
     * @param outstream output stream.
     * @param json_header JSON header which will be encrypted.
    */
    void stream_header(std::ostream& outstream, const std::string& json_header);


    /**
     * @brief Original filename which will be encrypted.
     * @param filename original filename.
     * @return returns encrypted filename.
    */
    std::string encrypt_filename(const std::string& filename);


    /**
     * @brief Generate a random filename.
     * @return returns the random filename.
    */
    std::string random_filename();


private:
    std::mutex _mutex;
};

} // namespace minilockcpp


#endif // _MINILOCKCPP_ENCRYPTION_H_