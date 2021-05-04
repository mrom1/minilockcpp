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


#ifndef _MINILOCKCPP_EXCEPTION_H_
#define _MINILOCKCPP_EXCEPTION_H_


#include <iostream>
#include <sstream>

namespace minilockcpp
{

/**
 * @brief Exception helper utility class.
 * @tparam exception_type the type of the exception.
*/
template<typename exception_type>
class exception_helper: public std::exception
{
public:
        /**
         * @brief Default constructor.
        */
        exception_helper() = default;


        /**
         * @brief Copy constructor.
         * @param source source object.
        */
        exception_helper(const exception_helper& source): stream(source.stream.str())
        {}


        /**
         * @brief Get the exception message.
         * @return returns the exception message.
        */
        virtual const char* what() const noexcept
        {
            if (msg_buffer.empty())
            {
                msg_buffer = stream.str();
            }
            return msg_buffer.c_str();
        }


        /**
         * @brief Overloaded streaming operator.
         * @tparam T Exception type.
         * @param v Exception type.
         * @return casted exception.
        */
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


} // namespace minilockcpp


#endif // _MINILOCKCPP_EXCEPTION_H_