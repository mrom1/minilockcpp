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


#ifndef _MINILOCKCPP_BASE58_H_
#define _MINILOCKCPP_BASE58_H_

#include <vector>
#include <string>

namespace minilockcpp
{

/**
 * @brief Static base58 utilities class for decoding and encoding.
*/
class base58 final
{
public:
    /**
     * @brief Encode vector of unsigned 8 bit integers to a string.
     * @param input vector of unsigned 8 bit integers.
     * @param result encoded result as a string.
     * @return returns true if successful, otherwise false.
    */
    static bool base58_encode(const std::vector<uint8_t>& input, std::string& result);

    /**
     * @brief Decode a string into a vector of unsigned 8 bit integers.
     * @param input string to decode.
     * @param result output vector.
     * @return returns true if successful, otherwise false.
    */
    static bool base58_decode(const std::string& input, std::vector<uint8_t>& result);

private:
    /**
     * @brief Prevent construction of this class. All methods are static.
    */
    base58() = default;
};

} // namespace minilockcpp


#endif // _MINILOCKCPP_BASE58_H_