######################################################################################
## Find 3rd party packages
######################################################################################
find_package(GTestSrc REQUIRED)
include(GoogleTest)


######################################################################################
## Build TestSuite
######################################################################################
add_executable(test_minilock
    test_base58.cc
    test_authentication.cc
    test_one_recipient.cc
    test_two_recipients.cc
    test_multithread.cc)
target_include_directories(test_minilock PRIVATE ${GTEST_INCLUDES} ${MINILOCK_INCLUDES} ${BLAKE2_INCLUDES})
target_link_libraries(test_minilock PRIVATE minilockcpp gtest_main)

enable_testing()

gtest_discover_tests(test_minilock)