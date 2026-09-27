cmake_minimum_required(VERSION 3.10)
project(Game_Live)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_DIR ${CMAKE_SOURCE_DIR}/lib/SDL)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)
add_subdirectory(${SDL2_DIR} ${CMAKE_BINARY_DIR}/SDL)

