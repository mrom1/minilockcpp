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



#include "logging.h"
#include <iomanip>
#include <chrono>

using namespace minilockcpp;

std::shared_ptr<minilockcpp::log> log::instance;

static std::ostream& add_time(std::ostream& stream)
{
    std::time_t helper = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#ifdef __linux__
    struct tm tm;
    localtime_r(&helper, &tm);
    stream << std::put_time(&tm, "%Y-%m-%d %T ");
#elif _WIN32
    struct tm* tm;
    tm = localtime(&helper);
    stream << std::put_time(tm, "%Y-%m-%d %T ");
#endif
    return stream;
}

std::ostream& log::error()
{
    return add_time(*get_instance().error_stream) << "[ERROR] ";
}
std::ostream& log::warning()
{
    return add_time(*get_instance().warning_stream) << "[WARNING] ";
}

std::ostream& log::info()
{
    return add_time(*get_instance().info_stream) << "[INFO] ";
}

std::ostream& log::verbose()
{
    return add_time(*get_instance().verbose_stream) << "[VERBOSE] ";
}

std::ostream& log::debug()
{
    return add_time(*get_instance().debug_stream) << "[DEBUG] ";
}

void log::set_log_level(log::log_level level)
{
    get_instance()._set_log_level(level);
}

log::log(): null_stream(&null_buffer)
{
}

minilockcpp::log& log::get_instance()
{
    if (!instance)
    {
        instance = std::shared_ptr<log>(new log);
    }
    return *instance.get();
}

void log::_set_log_level(log::log_level level)
{
    error_stream = &std::cerr;
    warning_stream = &std::cerr;
    info_stream = &std::cout;
    verbose_stream = &std::cout;
    debug_stream = &std::cout;
    switch (level)
    {
        case log_level::none:    error_stream = &null_stream;
        case log_level::error:   warning_stream = &null_stream;
        case log_level::warning: info_stream = &null_stream;
        case log_level::info:    verbose_stream = &null_stream;
        case log_level::verbose: debug_stream = &null_stream;
        default: break;
    }
}
