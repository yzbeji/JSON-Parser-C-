cmake_minimum_required(VERSION 3.10)


add_library(json json.cpp)
target_link_libraries(json parser)
target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)