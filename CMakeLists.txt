cmake_minimum_required(VERSION 3.2)
project(minilockcpp)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

cmake_policy(SET CMP0064 OLD)

find_package(Base58Src REQUIRED)
#find_package(Base64Src REQUIRED)
find_package(ScryptSrc REQUIRED)
find_package(Blake2Src REQUIRED)
find_package(SodiumSrc REQUIRED)
find_package(RapidJsonSrc REQUIRED)

set(MINILOCK_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lib"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_subdirectory(src)
add_subdirectory(test)
