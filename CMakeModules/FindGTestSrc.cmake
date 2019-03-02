
include(ExternalProject)

find_package(Threads)

SET(GTEST_SEARCH_PATH
    "${CMAKE_MODULE_PATH}/../extern/gtest")

ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    UPDATE_COMMAND ""
    PREFIX ${GTEST_SEARCH_PATH}
    INSTALL_COMMAND ""
)

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")

ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDES "${source_dir}/googletest/include")

ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBRARY ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${LIBSUFFIX})
set(GTEST_LIBRARY_MAIN ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${LIBSUFFIX})

add_library(gtest IMPORTED STATIC GLOBAL)

set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION                 "${GTEST_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES     "${GTEST_INCLUDES}"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")

add_library(gtest_main IMPORTED STATIC GLOBAL)
set_target_properties(gtest_main PROPERTIES
    IMPORTED_LOCATION "${GTEST_LIBRARY_MAIN}"
    IMPORTED_LINK_INTERFACE_LIBRARIES
        "${GTEST_LIBRARY};${CMAKE_THREAD_LIBS_INIT}")

add_dependencies(gtest GTestExternal)


#set(GTEST_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
#set(GTEST_LIBRARY gtest)
#add_library(${GTEST_LIBRARY} STATIC IMPORTED)

#add_library(gtest STATIC IMPORTED)
#add_library(gtest_main STATIC IMPORTED)

#set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION
#                ${GTEST_LIBRARY_PATH} )

#add_dependencies(${GTEST_LIBRARY} googletest)

# FindGTest.cmake
function(add_gtests executable timeout)
    if(NOT ARGN)
        message(FATAL_ERROR "Missing ARGN: Read the documentation for GTEST_ADD_TESTS")
    endif()
    if(ARGN STREQUAL "AUTO")
        # obtain sources used for building that executable
        get_property(ARGN TARGET ${executable} PROPERTY SOURCES)
    endif()
    set(gtest_case_name_regex ".*\\( *([A-Za-z_0-9]+), *([A-Za-z_0-9]+) *\\).*")
    set(gtest_test_type_regex "(TYPED_TEST|TEST_?[FP]?)")
    foreach(source ${ARGN})
        file(READ "${source}" contents)
        string(REGEX MATCHALL "${gtest_test_type_regex}\\(([A-Za-z_0-9 ,]+)\\)" found_tests ${contents})
        foreach(hit ${found_tests})
          string(REGEX MATCH "${gtest_test_type_regex}" test_type ${hit})

          # Parameterized tests have a different signature for the filter
          if(${test_type} STREQUAL "TEST_P")
            string(REGEX REPLACE ${gtest_case_name_regex}  "*/\\1.\\2/*" test_name ${hit})
          elseif(${test_type} STREQUAL "TEST_F" OR ${test_type} STREQUAL "TEST")
            string(REGEX REPLACE ${gtest_case_name_regex} "\\1.\\2" test_name ${hit})
          elseif(${test_type} STREQUAL "TYPED_TEST")
            string(REGEX REPLACE ${gtest_case_name_regex} "\\1/*.\\2" test_name ${hit})
          else()
            message(WARNING "Could not parse GTest ${hit} for adding to CTest.")
            continue()
          endif()
          add_test(NAME ${test_name} COMMAND ${executable} --gtest_filter=${test_name} ${extra_args})
          set_tests_properties(${test_name} PROPERTIES TIMEOUT ${timeout} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        endforeach()
    endforeach()
endfunction()

function(add_gtest name timeout)
    add_executable(${name} ${ARGN})
    target_compile_definitions(${name} PRIVATE -DUCRYPT_TEST_BUILD_DIR="${CMAKE_CURRENT_BINARY_DIR}")
    target_link_libraries(${name} gtest_main)
    add_gtests(${name} ${timeout} ${ARGN})
    add_custom_command(TARGET ${name} POST_BUILD COMMAND ctest)
endfunction()
