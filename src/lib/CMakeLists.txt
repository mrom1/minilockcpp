###########################################################################
## This file is part of the minilockcpp distribution 
## (https://github.com/mrom1/minilockcpp).
## Copyright (c) 2021 mrom1.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, version 3.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program. If not, see <http://www.gnu.org/licenses/>.
###########################################################################


###########################################################################
## find external dependencies
###########################################################################
find_package(Blake2 REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)


###########################################################################
## Build minilockcpp library
###########################################################################
add_library(minilockcpp STATIC
    blake2s_wrapper.hpp
    base58.h base58.cc
    base64.h base64.cc
    minilock.h
    minilock.cc
    constants.h
    header.h
    header.cc
    encryption.h
    encryption.cc
    decryption.h
    decryption.cc
    exception.h
    logging.h
    logging.cc
    utils.cc
    utils.h)

add_dependencies(minilockcpp libb2_SRC)
target_include_directories(minilockcpp PUBLIC ${BLAKE2_INCLUDES})
target_link_libraries(minilockcpp PUBLIC libb2 rapidjson unofficial-sodium::sodium)

