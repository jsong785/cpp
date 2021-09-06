cmake_minimum_required(VERSION 3.7)
project(word_trie)

include(../catch/catch_main.cmake)

add_executable(word_trie word_trie.cpp)
target_link_libraries(word_trie PRIVATE catch_main)

