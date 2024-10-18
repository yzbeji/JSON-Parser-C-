cmake_minimum_required(VERSION 3.10)

project(JsonEvaluator)

add_executable(json_eval main.cpp)
add_subdirectory(tokenizer)
target_link_libraries(json_eval PRIVATE tokenizer)
