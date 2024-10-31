cmake_minimum_required(VERSION 3.10)

project(testing)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(testing testing.cpp)
enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(testing PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(testing PRIVATE json arguments)
add_test(AllTestsInMain testing)