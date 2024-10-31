cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAX_CXX_STANDARD_REQUIRED True)


add_library(parser src/parser.cpp)
target_include_directories(parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(tokenizer)
target_link_libraries(parser PUBLIC tokenizer)
