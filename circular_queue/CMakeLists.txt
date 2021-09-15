cmake_minimum_required(VERSION 3.7)
project(circular_queue)

include(../catch/catch_main.cmake)

add_executable(circular_queue circular_queue.cpp)
target_link_libraries(circular_queue PRIVATE catch_main)

