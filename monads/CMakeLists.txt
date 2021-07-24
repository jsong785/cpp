cmake_minimum_required(VERSION 3.7)
project(monads)

include(../catch/catch_main.cmake)

add_executable(monads maybe.cpp)
target_link_libraries(monads PRIVATE catch_main)

