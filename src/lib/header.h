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


#ifndef _MINILOCKCPP_HEADER_H_
#define _MINILOCKCPP_HEADER_H_


#include "utils.h"
#include "constants.h"
#include <string>
#include <array>

namespace minilockcpp
{

/**
 * @brief JSON Header functionallity class.
*/
class header : public utils
{
protected:
    /**
     * @brief Parse / decrypt the encrypted JSON Header.
     * @param json_header JSON Header to be parsed.
     * @param secretkey pointer to the secret key array.
     * @return returns true if successful, otherwise false.
    */
    bool parse_header(const std::string& json_header,
                      const uint8_t* const secretkey);


    /**
     * @brief Generate a encrypted JSON Header.
     * @param sender_id ID of the owner or sender.
     * @param filehash pointer to the filehash array.
     * @param secretkey pointer to the secret key array.
     * @param recipients list of recipients.
     * @param is_own_recipient flag if the client encrypting the file
     *                         should also be added as a recipient.
     * @return returns the encrypted JSON Header.
    */
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

} // namespace minilockcpp


#endif // _MINILOCKCPP_HEADER_H_