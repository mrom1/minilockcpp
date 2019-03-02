#pragma once
#include <iostream>
#include <sstream>

namespace minilockcpp
{

template<typename exception_type>
class exception_helper: public std::exception
{
public:
        exception_helper() = default;
        exception_helper(const exception_helper& source): stream(source.stream.str())
        {}

        virtual const char* what() const noexcept
        {
            if (msg_buffer.empty())
            {
                msg_buffer = stream.str();
            }
            return msg_buffer.c_str();
        }

        template<typename T>
        exception_type& operator<<(const T& v)
        {
            stream << v;
            return *static_cast<exception_type*>(this);
        }

protected:
        std::ostringstream stream;
        mutable std::string msg_buffer;
};

struct general_error: public exception_helper<general_error> {};
struct general_decryption_error: public exception_helper<general_decryption_error> {};
struct general_encryption_error: public exception_helper<general_encryption_error> {};
struct file_not_found_error: public exception_helper<file_not_found_error> {};
struct io_error: public exception_helper<io_error> {};
struct initialize_error: public exception_helper<initialize_error> {};
struct json_parse_error: public exception_helper<json_parse_error> {};
struct invalid_minilock_version: public exception_helper<json_parse_error> {};
struct invalid_authentication: public exception_helper<invalid_authentication> {};


}
