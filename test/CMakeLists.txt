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
## Find 3rd party packages
###########################################################################
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)


###########################################################################
## Build TestSuite
###########################################################################
add_executable(test_minilock
    test_base58.cc
    test_authentication.cc
    test_one_recipient.cc
    test_two_recipients.cc
    test_multithread.cc)
target_include_directories(test_minilock PRIVATE ${MINILOCK_INCLUDES})
target_link_libraries(test_minilock PRIVATE minilockcpp GTest::gtest_main)

gtest_discover_tests(test_minilock)