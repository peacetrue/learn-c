cmake_minimum_required(VERSION 3.0)

include_directories(src/main/public) # 指定头文件目录
aux_source_directory(src/main/cpp LIBRARY_SRCS) # 指定源文件目录
add_library(library ${LIBRARY_SRCS})
