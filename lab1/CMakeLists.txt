cmake_minimum_required(VERSION 3.12)
project(lab1-crypto)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic")

find_package(fmt)

add_executable(main main.cpp Algorithms.cpp)
target_link_libraries(main fmt::fmt)