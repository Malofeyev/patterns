cmake_minimum_required(VERSION 3.16)

project(01)

add_library(sql_builder sql_query_builder.cpp)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE sql_builder)
