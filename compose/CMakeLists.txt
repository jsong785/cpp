cmake_minimum_required(VERSION 3.7)
project(compose)

include(../catch/catch_main.cmake)

add_executable(compose compose.cpp)
target_link_libraries(compose PRIVATE catch_main)

