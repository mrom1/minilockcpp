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