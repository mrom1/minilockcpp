find_package(GTestSrc REQUIRED)

include_directories(
    ${GTEST_INCLUDES}
    ${MINILOCK_INCLUDES}
    ${BLAKE2_INCLUDES}
)

add_gtest(test_minilock 240
    test_authentication.cc
    test_one_recipient.cc
    test_two_recipients.cc
    test_multithread.cc
)

enable_testing()

target_link_libraries(test_minilock minilockcpp)
