#pragma once

namespace minilockcpp
{

const char magic_bytes[] = {0x6d, 0x69, 0x6e, 0x69, 0x4c, 0x6f, 0x63, 0x6b};

enum minilock_constant {
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
            secretkey_bytes = 32
        };

        enum scrypt_constant {
            scrypt_cpu_ram_cost = 131072,
            scrypt_ram_cost = 8,
            scrypt_parallelisation = 1
        };
}
