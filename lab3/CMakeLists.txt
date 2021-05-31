cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt)

add_executable(lab3 main.cpp)
target_link_libraries(lab3 fmt::fmt)