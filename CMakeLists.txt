cmake_minimum_required(VERSION 3.10)

project(main)
set(CMAKE_CXX_STANDARD 20)
set(CMAX_CXX_STANDARD_REQUIRED True)
add_executable(main main.cpp)
add_subdirectory(json)
add_subdirectory(arguments)
target_link_libraries(main PRIVATE json arguments)


