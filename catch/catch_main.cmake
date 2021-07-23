include(../compile/compile.cmake)

add_library(catch_main INTERFACE)
target_sources(catch_main INTERFACE ${CMAKE_CURRENT_LIST_DIR}/catch.hpp ${CMAKE_CURRENT_LIST_DIR}/catch_main.cpp)
target_include_directories(catch_main INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(catch_main INTERFACE compile)
