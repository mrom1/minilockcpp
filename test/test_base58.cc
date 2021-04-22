#include <base58.h>

#include <gtest/gtest.h>

#include <string>
#include <iostream>
#include <vector>

TEST(test_base58, test_base58_encode)
{
    const std::string teststring = "some test string to be encoded";
    const std::vector<uint8_t> vTeststring(teststring.begin(), teststring.end());
    std::string result;
    bool encodeResult = minilockcpp::base58::base58_encode(vTeststring, result);
    EXPECT_EQ(encodeResult, true);
    EXPECT_EQ(result, "Q8YhYUJn3uc7zkfABs7btWDtFgH8pLgUjpfSBGaPm");
}

TEST(test_base58, test_base58_decode)
{
    std::vector<unsigned char> retBuffer;
    const std::string teststring = "Q8YhYUJn3uc7zkfABs7btWDtFgH8pLgUhYhd8kEVH";

    bool result = minilockcpp::base58::base58_decode(teststring.c_str(), retBuffer);
    EXPECT_EQ(result, true);

    std::string decoded_string(retBuffer.begin(), retBuffer.end());
    EXPECT_EQ(decoded_string, "some test string to be decoded");
}