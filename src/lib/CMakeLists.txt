######################################################################################
## Build minilockcpp library
######################################################################################
add_library(minilockcpp STATIC
    base58.h base58.cc
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
target_include_directories(minilockcpp PRIVATE 
    ${RAPIDJSON_INCLUDES}
    #${BASE58_INCLUDES}
    #${SCRYPT_INCLUDES}
    ${BLAKE2_INCLUDES}
    ${SODIUM_INCLUDES})
target_link_libraries(minilockcpp stdc++fs
    ${BLAKE2_LIBRARY}
    ${SODIUM_LIBRARY}
    #${SCRYPT_LIBRARY}
    #${BASE58_LIBRARY}
    #${BASE64_LIBRARY}
)
#target_compile_definitions(minilockcpp PRIVATE BUFFERSIZE=${BASE64_BUFFER_SIZE})
