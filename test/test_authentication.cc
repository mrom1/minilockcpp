#include <minilock.h>
#include <blake2.h>
#include "gtest/gtest.h"
#include <exception.h>

using namespace minilockcpp;

TEST(test_authentication, test_initialization_error)
{
    try
    {
        minilock first_session;
        first_session.set_log_level(log::log_level::none);
        ASSERT_THROW(first_session.encrypt_file("dummy", "dummy"), initialize_error);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
}

TEST(test_authentication, test_invalid_password)
{
    try
    {
        std::string filename = "random_1MB";
        std::string filename_encrypted = "random_1MB.minilock";
        std::string correct_password = "cytosine stratigraphic megajoules relaxingly dourly timing contractions";
        std::string invalid_password = "cytosine stratigraphic megajoules relaxingly dourly timing different";

        std::vector<unsigned char> data(1000*1000);
        std::generate(begin(data), end(data), std::rand);

        std::ofstream ofs(filename, std::ios::binary);
        ofs.write(reinterpret_cast<char*>(&data[0]), static_cast<long>(data.size()));
        ofs.close();

        minilock first_session("asdf@asdf.de", correct_password);
        EXPECT_TRUE(first_session.encrypt_file(filename));
        remove(filename.c_str());

        minilock second_session;
        second_session.set_log_level(log::log_level::none);
        second_session.initialize("asdf@asdf.de", invalid_password);
        EXPECT_THROW(second_session.decrypt_file(filename_encrypted), invalid_authentication);

        remove(filename.c_str());
        remove(filename_encrypted.c_str());
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
}

TEST(test_authentication, test_user_id)
{
    try
    {
        std::string first_expected_id = "8FLEXeQ9nBGNy2sSKqfe97C4QtATyosbhHLzyvfBDxkGP";
        std::string second_expected_id = "Le5X8i3aRR2cadb7E9YXbr993DDKSN7wxgh9rkRyGfSo2";
        minilock first_session("asdf@asdf.de", "cytosine stratigraphic megajoules relaxingly dourly timing contractions");
        EXPECT_TRUE(first_session.get_id() == first_expected_id);

        minilock second_session("xyz@gmail.com", "hunting courtesy banquet hostility demand dressing sharp sow create floor");
        EXPECT_TRUE(second_session.get_id() == second_expected_id);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
}

TEST(test_authentication, test_relog_user)
{
    try
    {
        bool result;
        minilock *session = nullptr;
        ASSERT_NO_THROW(session = new minilock());
        ASSERT_NO_THROW(result = session->initialize("asdf@asdf.de","cytosine stratigraphic megajoules relaxingly dourly timing contractions"));
        ASSERT_TRUE(result);
        if(session != nullptr)
            delete session;

        ASSERT_NO_THROW(session = new minilock());
        ASSERT_NO_THROW(result = session->initialize("asdf@asdf.de","cytosine stratigraphic megajoules relaxingly dourly timing contractions"));
        ASSERT_TRUE(result);
        if(session != nullptr)
            delete session;

    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        FAIL();
    }
}
