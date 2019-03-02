#include <minilock.h>
#include <blake2.h>
#include "gtest/gtest.h"
#include "tux_image.h"

#include <iostream>
#include <vector>
#include <random>

using namespace minilockcpp;


TEST(test_one_recipient, test_tux_image)
{
    try
    {
        bool result = false;
        std::string filename = "tux_image.png";
        std::string filename_encrypted = "tux_image.png.minilock";

        std::array<unsigned char, blake2s_constant::BLAKE2S_OUTBYTES> hash_original;
        std::array<unsigned char, blake2s_constant::BLAKE2S_OUTBYTES> hash_decrypted;

        std::ofstream ofs(filename, std::ios::binary);
        ofs.write(reinterpret_cast<char*>(tux_image_png), tux_image_png_len);
        ofs.close();

        blake2s(&hash_original[0], tux_image_png, nullptr, hash_original.size(), tux_image_png_len, 0);

        minilock session;
        ASSERT_NO_THROW(result = session.initialize("asdf@asdf.de","cytosine stratigraphic megajoules relaxingly dourly timing contractions"));
        ASSERT_TRUE(result);

        ASSERT_NO_THROW(result = session.encrypt_file(filename));
        EXPECT_TRUE(result);
        remove(filename.c_str());
        ASSERT_NO_THROW(result = session.decrypt_file(filename_encrypted));
        EXPECT_TRUE(result);

        std::ifstream ifs(filename, std::ios::binary);
        std::string decrypted_file ((std::istreambuf_iterator<char>(ifs) ),
                                    (std::istreambuf_iterator<char>()    ) );
        ifs.close();
        remove(filename.c_str());
        remove(filename_encrypted.c_str());

        blake2s(&hash_decrypted[0], decrypted_file.c_str(), nullptr, hash_decrypted.size(), decrypted_file.size(), 0);

        ASSERT_TRUE(hash_original == hash_decrypted);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
}


TEST(test_one_recipient, test_10mb_random)
{
    try
    {
        bool result = false;
        std::string filename = "random_10MB";
        std::string filename_encrypted = "random_10MB.minilock";

        std::array<unsigned char, blake2s_constant::BLAKE2S_OUTBYTES> hash_original;
        std::array<unsigned char, blake2s_constant::BLAKE2S_OUTBYTES> hash_decrypted;

        std::vector<unsigned char> data(1000*1000*10);
        std::generate(begin(data), end(data), std::rand);

        std::ofstream ofs(filename, std::ios::binary);
        ofs.write(reinterpret_cast<char*>(&data[0]), static_cast<long>(data.size()));
        ofs.close();

        blake2s(&hash_original[0], &data[0], nullptr, hash_original.size(), data.size(), 0);

        minilock session;
        ASSERT_NO_THROW(result = session.initialize("asdf@asdf.de","cytosine stratigraphic megajoules relaxingly dourly timing contractions"));
        ASSERT_TRUE(result);

        ASSERT_NO_THROW(result = session.encrypt_file(filename));
        EXPECT_TRUE(result);
        remove(filename.c_str());
        ASSERT_NO_THROW(result = session.decrypt_file(filename_encrypted));
        EXPECT_TRUE(result);

        std::ifstream ifs(filename, std::ios::binary);
        std::string decrypted_file ((std::istreambuf_iterator<char>(ifs) ),
                                    (std::istreambuf_iterator<char>()    ) );
        ifs.close();
        remove(filename.c_str());
        remove(filename_encrypted.c_str());

        blake2s(&hash_decrypted[0], decrypted_file.c_str(), nullptr, hash_decrypted.size(), decrypted_file.size(), 0);

        ASSERT_TRUE(hash_original == hash_decrypted);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
}
