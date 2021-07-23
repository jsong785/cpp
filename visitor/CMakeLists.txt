cmake_minimum_required(VERSION 3.7)
project(visitor)

include(../catch/catch_main.cmake)

add_executable(visitor visitor.cpp)
target_link_libraries(visitor PRIVATE catch_main)

