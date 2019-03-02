
include(ExternalProject)

SET(BLAKE2_SEARCH_PATH
    "${CMAKE_MODULE_PATH}/../extern/libb2")

ExternalProject_Add(
    libb2
    GIT_REPOSITORY https://github.com/BLAKE2/libb2.git
    #GIT_TAG stable
    UPDATE_COMMAND ""
    PREFIX ${BLAKE2_SEARCH_PATH}
    SOURCE_DIR "${BLAKE2_SEARCH_PATH}/src/libb2"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure
    BUILD_COMMAND make
    INSTALL_COMMAND ""
)

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Get_Property(libb2 source_dir)
set(BLAKE2_INCLUDES "${source_dir}/src")
    
ExternalProject_Get_Property(libb2 binary_dir)

set(BLAKE2_LIBRARY ${binary_dir}/src/.libs/${CMAKE_FIND_LIBRARY_PREFIXES}b2${LIBSUFFIX})

add_library(b2 IMPORTED STATIC GLOBAL)

set_target_properties(libb2 PROPERTIES
    IMPORTED_LOCATION                 "${BLAKE2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES     "${BLAKE2_INCLUDES}")

