#pragma once

#include <sstream>
#include <iostream>
#include <memory>

namespace minilockcpp
{

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

}
