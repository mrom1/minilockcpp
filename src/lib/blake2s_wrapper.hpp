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


#ifndef _MINILOCKCPP_BLAKE2S_WRAPPER_H_
#define _MINILOCKCPP_BLAKE2S_WRAPPER_H_


#include <blake2.h>

namespace minilockcpp
{

/**
 * @brief Basic wrapper for using different versions of the blake2 library.
 *        Windows uses a older version of the blake2 library, the API is
 *        slightly different. Therefore a wrapper is necessary.
 *        This should be fixed in the future.
 */
class blake2s_wrapper final
{
public:

    /**
     * @brief Create a blake2s hash.
     * @param out pointer to output array
     * @param in pointer to input array
     * @param key pointer to salt key array
     * @param outlen array output size
     * @param inlen array input size
     * @param keylen salt array size
     * @return returns '-1' on failure 
    */
    static int blake2s_hash(uint8_t* out, const void* in, const void* key, size_t outlen, size_t inlen, size_t keylen)
    {
#ifdef _WIN32
        return blake2s(out, outlen, in, inlen, key, keylen);
#elif __linux__
        return blake2s(out, in, key, outlen, inlen, keylen);
#else
        return -1;
#endif
    }

private:
    /**
     * @brief Prevent construction of this class. All methods are static.
    */
    blake2s_wrapper() = default;
};

} // namespace minilockcpp

#endif // _MINILOCKCPP_BLAKE2S_WRAPPER_H_