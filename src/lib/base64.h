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


#ifndef _MINILOCKCPP_BASE64_H_
#define _MINILOCKCPP_BASE64_H_


#include <vector>
#include <string>

namespace minilockcpp
{

/**
 * @brief Static base64 utilities class for decoding and encoding.
*/
class base64 final
{
public:
    /**
     * @brief Encode unsigned 8 bit integers to a string.
     * @param bytes_to_encode pointer to unsigned 8 bit integer array.
     * @param in_len length of input array.
     * @return returns base64 encoded string.
    */
    static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);

    /**
     * @brief Decode encoded base64 string.
     * @param encoded_string string to decode.
     * @return returns decoded base64 string.
    */
    static std::string base64_decode(std::string const& encoded_string);

private:
    /**
     * @brief Prevent construction of this class. All methods are static.
    */
    base64() = default;
};

} // namespace minilockcpp


#endif // _MINILOCKCPP_BASE64_H_