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


#ifndef _MINILOCKCPP_CONSTANTS_H_
#define _MINILOCKCPP_CONSTANTS_H_


namespace minilockcpp
{

const char magic_bytes[] = {0x6d, 0x69, 0x6e, 0x69, 0x4c, 0x6f, 0x63, 0x6b};

enum minilock_constant 
{
    filename_padding = 256,
    filekey_bytes = 32,
    filenonce_bytes = 16,
    fullnonce_bytes = 24,
    padding_bytes = 16,
    chunk_number_bytes = 8,
    chunk_array_length = 4,
    chunk_size = 1048576,
    magicbytes_length = 8,
    uid_checksum_length = 1,
    publickey_bytes = 32,
    secretkey_bytes = 32,
    blake2s_hashbytes = 32
};

enum scrypt_constant 
{
    scrypt_cpu_ram_cost = 131072,
    scrypt_ram_cost = 8,
    scrypt_parallelisation = 1
};

} // namespace minilockcpp

#endif // _MINILOCKCPP_CONSTANTS_H_
