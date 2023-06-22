cmake_minimum_required(VERSION 3.20)
# delegating constructors are permitted only in C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-fdeclspec)

project(learn-c)

add_subdirectory(src/lib)
add_subdirectory(src/main)
add_subdirectory(src/test)
add_subdirectory(src/elf)
add_subdirectory(src/case)
