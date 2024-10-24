cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(argument argument.cpp "argument.h")	
target_include_directories(argument PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(argument PUBLIC json)
target_link_libraries(argument PUBLIC parser)