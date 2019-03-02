
include_directories(
    ${RAPIDJSON_INCLUDES}
    ${BASE58_INCLUDES}
    ${SCRYPT_INCLUDES}
    ${BLAKE2_INCLUDES}
    ${SODIUM_INCLUDES}
)

add_library(minilockcpp STATIC
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
    utils.h
)

target_compile_definitions(minilockcpp PRIVATE BUFFERSIZE=${BASE64_BUFFER_SIZE})

target_link_libraries(minilockcpp stdc++fs
    ${BLAKE2_LIBRARY}
    ${SODIUM_LIBRARY}
    ${SCRYPT_LIBRARY}
    ${BASE58_LIBRARY}
    ${BASE64_LIBRARY}
)
