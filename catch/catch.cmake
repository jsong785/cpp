add_library(catch_main OBJECT catch.hpp catch_main.cpp)
target_include_directories(catch_main PUBLIC ${CMAKE_SOURCE_DIR})
