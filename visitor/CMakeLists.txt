include(../catch/catch.cmake)

add_executable(visitor visitor.cpp)
target_link_libraries(visitor PRIVATE cmake)

