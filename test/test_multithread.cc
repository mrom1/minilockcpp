#include <minilock.h>
#include <blake2.h>
#include "gtest/gtest.h"

#include <iostream>
#include <vector>
#include <random>
#include <thread>


using namespace minilockcpp;
using hash_array = std::array<unsigned char, blake2s_constant::BLAKE2S_OUTBYTES>;

void generate_file(const std::string& filename, hash_array& filehash)
{
    std::vector<unsigned char> data(1000*1000*10);
    std::generate(begin(data), end(data), std::rand);

    std::ofstream ofs(filename, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(&data[0]), static_cast<long>(data.size()));
    blake2s(filehash.data(), data.data(), nullptr, filehash.size(), data.size(), 0);
    ofs.close();
}

void read_filehash(const std::string& filename, hash_array& filehash)
{
    std::ifstream ifs(filename, std::ios::binary);
    std::vector<char> data(( std::istreambuf_iterator<char>(ifs)),
                                    std::istreambuf_iterator<char>());
    blake2s(filehash.data(), data.data(), nullptr, filehash.size(), data.size(), 0);
    ifs.close();
}

TEST(test_multithread, test_random_files)
{
    try
    {
        bool result = false;
        const size_t files_count = 10;
        std::string filename = "random_10MB";
        std::string filename_encrypted_suffix = ".minilock";

        std::map<std::string, hash_array> files_original;
        std::map<std::string, hash_array> files_decrypted;

        std::vector<std::thread> threads;

        minilock session;
        ASSERT_NO_THROW(result = session.initialize("asdf@asdf.de","cytosine stratigraphic megajoules relaxingly dourly timing contractions"));
        ASSERT_TRUE(result);

        for(size_t i = 0; i < files_count; i++)
        {
            hash_array filehash;
            generate_file(filename + "_" + std::to_string(i), filehash);
            files_original.emplace(std::make_pair(filename + "_" + std::to_string(i),filehash));
        }

        for(auto it = files_original.begin(); it != files_original.end(); it++)
        {
            threads.push_back(std::thread([&session, it, &result]()
            {
                ASSERT_NO_THROW(result = session.encrypt_file(it->first.c_str()));
                EXPECT_TRUE(result);
                remove(it->first.c_str());
            }));
        }

        std::for_each(threads.begin(), threads.end(), [](std::thread& t)
        {
           t.join();
        });

        threads.clear();

        for(size_t i = 0; i < files_count; i++)
        {
            threads.push_back(std::thread([&session, i, &result, filename, &files_decrypted]()
            {
                hash_array filehash;
                std::string filename_encrypted = filename + "_" + std::to_string(i) + ".minilock";
                std::string filename_original = filename + "_" + std::to_string(i);

                ASSERT_NO_THROW(result = session.decrypt_file(filename_encrypted));
                EXPECT_TRUE(result);
                read_filehash(filename_original, filehash);
                files_decrypted.emplace(std::make_pair(filename_original, filehash));

                remove(filename_original.c_str());
                remove(filename_encrypted.c_str());

            }));
        }

        std::for_each(threads.begin(), threads.end(), [](std::thread& t)
        {
           t.join();
        });

        ASSERT_TRUE(files_original == files_decrypted);

    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
