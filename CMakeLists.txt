cmake_minimum_required(VERSION 3.21)
project(Bech32m)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(src)
add_subdirectory(test)
