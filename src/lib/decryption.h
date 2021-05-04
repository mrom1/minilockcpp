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


#ifndef _MINILOCKCPP_DECRYPTION_H_
#define _MINILOCKCPP_DECRYPTION_H_


#include "header.h"
#include <mutex>

namespace minilockcpp 
{

/**
 * @brief Decryption functionallity class.
*/
class decryption : public header
{
protected:
    /**
     * @brief Decryption of a file
     * @param source_filename filename string without path.
     * @param secretkey pointer to the secretkey used for decryption.
     * @param source_directory path of the directory for the file to be decrypted.
     * @param target_directory path where the decrypted file will be saved to.
    */
    void decrypt(const std::string& source_filename,
                 const uint8_t* const secretkey,
                 const std::string& source_directory={},
                 const std::string& target_directory={});

private:
    /**
     * @brief Utility function. Decrypt a filestream.
     * @param instream input stream.
     * @param outstream output stream.
     * @param pos_end ending position. Default start is '0'.
    */
    void decrypt_stream(std::istream& instream,
                        std::ostream& outstream,
                        uint64_t pos_end = 0);


    /**
     * @brief Utility function. Finde the JSON header in the file
     *        which should be decrypted.
     * @param is input stream.
     * @return returns the JSON header.
    */
    std::string find_header(std::istream& is);


    /**
     * @brief The original filename can be encrypted. This function
     *        will return the original decrypted filename.
     * @param is input stream.
     * @return returns the decrypted filename.
    */
    std::string decrypt_filename(std::istream& is);


private:
    std::mutex _mutex;
};

} // namespace minilockcpp


#endif // _MINILOCKCPP_DECRYPTION_H_