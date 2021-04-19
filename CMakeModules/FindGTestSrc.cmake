include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)


# No need to have GTest in the install script
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# No need to build the mocking library at this point
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

if(WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(WIN32)

FetchContent_MakeAvailable(googletest)
