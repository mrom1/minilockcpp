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


#ifndef _MINILOCKCPP_UTILS_H_
#define _MINILOCKCPP_UTILS_H_


#include <cstdint>
#include <string>
#include <sstream>

#include <vector>
#include <array>

namespace minilockcpp
{
/**
 * @brief Utility class with functions for file handling and writing / reading chunks.
*/
class utils
{
protected:
    /**
     * @brief Open a filestream of a given file.
     * @param file filestream.
     * @param flags file mode (read, write).
     * @param filename filename without a leading path.
     * @param working_directory path to the directory containing the file.
     *                          If empty the current path will be used.
    */
    void open_file(std::fstream& file, const std::ios_base::openmode& flags, const std::string& filename, const std::string& working_directory);


    /**
     * @brief Close a filestream.
     * @param file filestream.
    */
    void close_file(std::fstream& file);


    /**
     * @brief Remove a file from disk.
     * @param filename filename to be removed without a leading path.
     * @param working_directory path to the directory containing the file.
     *                          If empty the current path will be used.
    */
    void remove_file(const std::string& filename, const std::string& working_directory);


    /**
     * @brief Increment chunk counter depending on the nonce.
     * @param fullNonce pointer to the array containing the full nonce.
    */
    static void incrementChunkCounter(uint8_t *fullNonce);


    /**
     * @brief Read the length of a chunk.
     * @param data pointer to the data.
     * @param szData size of the data.
     * @param offset current offset in the file.
     * @return returns the length of the chunk.
    */
    static int readChunkLength(uint8_t *data, size_t szData, size_t offset);


    /**
     * @brief Write the length of a chunk.
     * @param data pointer to the data.
     * @param szData size of the data.
     * @param chunkLen chunk length
    */
    static int writeChunkLength(uint8_t *data, size_t szData, size_t chunkLen);

    /**
     * @brief A flag has to be set in the full nonce on the last chunk.
     *        This function will write this flag to the full nonce.
     * @param fullNonce pointer to the full nonce.
    */
    static void setLastChunkFlag(uint8_t *fullNonce);
};

} // namespace minilockcpp

#endif // _MINILOCKCPP_UTILS_H_