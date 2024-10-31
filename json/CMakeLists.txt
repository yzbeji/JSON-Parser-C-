cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAX_CXX_STANDARD_REQUIRED True)

add_library(json src/json.cpp)
target_include_directories(json PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 
add_subdirectory(parser)
target_link_libraries(json PUBLIC parser)
target_link_libraries(json PUBLIC tokenizer)
