cmake_minimum_required(VERSION 3.0)

include_directories(../team-a/src/main/public) # 指定头文件目录
aux_source_directory(src/main/cpp APPLICATION_SRCS) # 指定源文件目录
add_executable(application ${APPLICATION_SRCS})
target_link_libraries(application library)
