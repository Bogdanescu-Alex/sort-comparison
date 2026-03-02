cmake_minimum_required(VERSION 3.10)

project(sortComparison)

add_executable(main main.c)

target_compile_options(main PRIVATE -O3)