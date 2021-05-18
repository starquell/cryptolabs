cmake_minimum_required(VERSION 3.17)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 fmt::fmt)