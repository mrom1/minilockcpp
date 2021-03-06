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



#include "base58.h"
#include <string.h>

using namespace minilockcpp;

static const char* base58Characters = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool base58::base58_encode(const std::vector<uint8_t>& input, std::string& result)
{
    const unsigned char* begin_ptr = input.data();
    const unsigned char* end_ptr = input.data() + input.size();

    int zeroes = 0;
    int length = 0;

    while (begin_ptr != end_ptr && *begin_ptr == 0) 
    {
        begin_ptr++;
        zeroes++;
    }

    int size = (end_ptr - begin_ptr) * 138 / 100 + 1; // log(256) / log(58), rounded up.
    std::vector<unsigned char> b58(size);

    while (begin_ptr != end_ptr) 
    {
        int carry = *begin_ptr;
        int i = 0;

        for (auto it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) 
        {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }

        if(carry != 0) return false;

        length = i;
        begin_ptr++;
    }

    auto it = b58.begin() + (size - length);

    while (it != b58.end() && *it == 0)
    {
        it++;
    }


    result.reserve(zeroes + (b58.end() - it));
    result.assign(zeroes, '1');

    while (it != b58.end())
    {
        result += base58Characters[*(it++)];
    }

    return true;
}

bool base58::base58_decode(const std::string& input, std::vector<uint8_t>& result)
{
    const char* psz = input.c_str();

    while (*psz && isspace(*psz))
    {
        psz++;
    }

    int zeroes = 0;
    int length = 0;

    while (*psz == '1') 
    {
        zeroes++;
        psz++;
    }

    int size = strlen(psz) * 733 /1000 + 1; // log(58) / log(256), rounded up.
    std::vector<unsigned char> b256(size);

    while (*psz && !isspace(*psz)) 
    {

        const char* ch = strchr(base58Characters, *psz);
        if (ch == nullptr)
            return false;

        int carry = ch - base58Characters;
        int i = 0;
        for (auto it = b256.rbegin(); (carry != 0 || i < length) && (it != b256.rend()); ++it, ++i) 
        {
            carry += 58 * (*it);
            *it = carry % 256;
            carry /= 256;
        }

        if(carry != 0) return false;

        length = i;
        psz++;
    }

    while (isspace(*psz))
    {
        psz++;
    }

    if (*psz != 0)
    {
        return false;
    }

    auto it = b256.begin() + (size - length);
    
    while (it != b256.end() && *it == 0)
    {
        it++;
    }

    result.reserve(zeroes + (b256.end() - it));
    result.assign(zeroes, 0x00);

    while (it != b256.end())
    {
        result.push_back(*(it++));
    }

    return true;
}
