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


#ifndef _MINILOCKCPP_LOGGING_H_
#define _MINILOCKCPP_LOGGING_H_


#include <sstream>
#include <iostream>
#include <memory>


namespace minilockcpp
{

/**
 * @brief Logging utility class.
 *        
 *        Log levels:
 *        None: Silent
 *        Error: Only error messages will be logged.
 *        Warning: Error and warning messages will be logged.
 *        Info: Error, warning and info messages will be logged.
 *        Verbose: Error, warning, info, and verbose messages will be logged.
 *        Debug: All messages and debug messages will be logged.
 * 
 *        Setting the log level:
 *        log::log_level level = log::log_level::info;
 *        log::set_log_level(level);
 * 
 *        How to use:
 *        log::info() << "Information to share\n";
*/
class log
{
public:
        enum class log_level
        {
            debug = 0,
            verbose,
            info,
            warning,
            error,
            none
        };

public:
        static std::ostream& error();
        static std::ostream& warning();
        static std::ostream& info();
        static std::ostream& verbose();
        static std::ostream& debug();
        static void set_log_level(log_level level);

    private:
        log();
        static log& get_instance();
        void _set_log_level(log_level level);

    private:
        static std::shared_ptr<log> instance;

        class null_stream_buffer : public std::streambuf
        {
            public:
                int overflow(int c) { return c; }
        };
        null_stream_buffer null_buffer;
        std::ostream null_stream;

        std::ostream* error_stream = &std::cerr;
        std::ostream* warning_stream = &std::cerr;
        std::ostream* info_stream = &std::cout;
        std::ostream* verbose_stream = &null_stream;
        std::ostream* debug_stream = &std::cout;
};

} // namespace minilockcpp


#endif // _MINILOCKCPP_LOGGING_H_