cmake_minimum_required(VERSION 3.21)
project(Bech32m)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    SET(CMAKE_CXX_BUILD_FLAGS  "/Wall")
else()
    SET(CMAKE_CXX_BUILD_FLAGS  "-Wall -Wextra")
endif()

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_BUILD_FLAGS}")

add_subdirectory(src)
add_subdirectory(test)
