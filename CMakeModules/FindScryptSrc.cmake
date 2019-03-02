include(ExternalProject)

SET(SCRYPT_SEARCH_PATH
    "${CMAKE_MODULE_PATH}/../extern/libscrypt")

ExternalProject_Add(libscrypt
  GIT_REPOSITORY https://github.com/technion/libscrypt.git
  #GIT_TAG stable
  PREFIX ${SCRYPT_SEARCH_PATH}
  UPDATE_COMMAND ""
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make
  INSTALL_COMMAND ""
)

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Get_Property(libscrypt source_dir)

set(SCRYPT_INCLUDES "${source_dir}")
    
ExternalProject_Get_Property(libscrypt binary_dir)
set(SCRYPT_LIBRARY ${binary_dir}/${LIBPREFIX}scrypt${LIBSUFFIX})

set_target_properties(libscrypt PROPERTIES
    IMPORTED_LOCATION                 "${SCRYPT_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES     "${SCRYPT_INCLUDES}")
