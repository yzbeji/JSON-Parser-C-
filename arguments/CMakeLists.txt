cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(arguments src/argument.cpp)
target_include_directories(arguments PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "../json")	
target_link_libraries(arguments PRIVATE json parser)	