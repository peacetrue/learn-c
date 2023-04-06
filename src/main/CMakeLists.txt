include_directories(../lib/public) # 指定头文件目录
aux_source_directory(cpp MAIN_SRCS) # 指定源文件目录
add_executable(main ${MAIN_SRCS})
target_link_libraries(main lib)
