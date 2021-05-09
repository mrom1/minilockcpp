include(ExternalProject)

SET(BLAKE2_SEARCH_PATH
    "${CMAKE_BINARY_DIR}/third-party/libb2")

if(WIN32)
    ExternalProject_Add(
        libb2_SRC
        GIT_REPOSITORY https://github.com/mrom1/libb2
        GIT_TAG feature/cmake
        UPDATE_COMMAND ""
        PREFIX ${BLAKE2_SEARCH_PATH}
        SOURCE_DIR "${BLAKE2_SEARCH_PATH}/src/libb2"
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
    )
endif(WIN32)

if(UNIX)
    ExternalProject_Add(
        libb2_SRC
        GIT_REPOSITORY https://github.com/BLAKE2/libb2.git
        UPDATE_COMMAND ""
        PREFIX ${BLAKE2_SEARCH_PATH}
        SOURCE_DIR "${BLAKE2_SEARCH_PATH}/src/libb2"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure
        BUILD_COMMAND make
        INSTALL_COMMAND ""
    )
endif(UNIX)

set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Get_Property(libb2_SRC source_dir)
set(BLAKE2_INCLUDES "${source_dir}/src")
    
ExternalProject_Get_Property(libb2_SRC binary_dir)

add_library(libb2 STATIC IMPORTED)

if(MSVC)
    set(BLAKE2_LIBRARY_DEBUG ${binary_dir}/lib/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}libb2${LIBSUFFIX})
    set(BLAKE2_LIBRARY_RELEASE ${binary_dir}/lib/Release/${CMAKE_FIND_LIBRARY_PREFIXES}libb2${LIBSUFFIX})
    set_target_properties(libb2 PROPERTIES IMPORTED_LOCATION_DEBUG "${BLAKE2_LIBRARY_DEBUG}")
    set_target_properties(libb2 PROPERTIES IMPORTED_LOCATION_RELEASE "${BLAKE2_LIBRARY_RELEASE}")
else()
    set(BLAKE2_LIBRARY ${binary_dir}/src/.libs/${CMAKE_FIND_LIBRARY_PREFIXES}b2${LIBSUFFIX})
    set_target_properties(libb2 PROPERTIES IMPORTED_LOCATION "${BLAKE2_LIBRARY}")
endif(MSVC)


