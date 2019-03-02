
include(ExternalProject)

SET(BASE58_SEARCH_PATH
    "${CMAKE_MODULE_PATH}/../extern/libbase58")

ExternalProject_Add(
    libbase58
    GIT_REPOSITORY https://github.com/luke-jr/libbase58.git
    #GIT_TAG stable
    UPDATE_COMMAND ""
    PREFIX ${BASE58_SEARCH_PATH}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure
    BUILD_COMMAND make
    INSTALL_COMMAND ""
)

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Get_Property(libbase58 source_dir)
set(BASE58_INCLUDES "${source_dir}")
    
ExternalProject_Get_Property(libbase58 binary_dir)

set(BASE58_LIBRARY ${binary_dir}/.libs/${LIBPREFIX}base58${LIBSUFFIX})

add_library(base58 IMPORTED STATIC GLOBAL)

set_target_properties(libbase58 PROPERTIES
    IMPORTED_LOCATION                 "${BASE58_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES     "${BASE58_INCLUDES}")

