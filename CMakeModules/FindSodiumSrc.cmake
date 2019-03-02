
include(ExternalProject)

SET(SODIUM_SEARCH_PATH
    "${CMAKE_MODULE_PATH}/../extern/libsodium")

ExternalProject_Add(
    libsodium
    GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
    GIT_TAG stable
    UPDATE_COMMAND ""
    PREFIX ${SODIUM_SEARCH_PATH}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure
    BUILD_COMMAND make
    INSTALL_COMMAND ""
)

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Get_Property(libsodium source_dir)
set(SODIUM_INCLUDES "${source_dir}/src/libsodium/include")
    
ExternalProject_Get_Property(libsodium binary_dir)

set(SODIUM_LIBRARY ${binary_dir}/src/libsodium/.libs/${LIBPREFIX}sodium${LIBSUFFIX})

add_library(sodium IMPORTED STATIC GLOBAL)

set_target_properties(libsodium PROPERTIES
    IMPORTED_LOCATION                 "${SODIUM_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES     "${SODIUM_INCLUDES}")

