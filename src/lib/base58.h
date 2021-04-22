#pragma once

#include <vector>
#include <string>

namespace minilockcpp
{

class base58 final
{
public:
    static bool base58_encode(const std::vector<unsigned char>& input, std::string& result);
    static bool base58_decode(const std::string& input, std::vector<unsigned char>& vch);

private:
    base58() = default;
};

}