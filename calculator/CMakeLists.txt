cmake_minimum_required(VERSION 3.7)
project(calculator)

include(../catch/catch_main.cmake)

add_executable(calculator calculator.cpp)
target_link_libraries(calculator PRIVATE catch_main)

