include_directories(../lib/public) # 指定头文件目录
#aux_source_directory(cpp MAIN_SRCS) # 指定源文件目录
add_executable(main cpp/main.cpp)
target_link_libraries(main lib)

# 添加一个 assembly 可执行文件
add_executable(assembly cpp/assembly.cpp)
## 保存汇编过程中生成的临时文件；设置不优化代码，以便于分析汇编文件
set_target_properties(assembly PROPERTIES COMPILE_FLAGS "-save-temps -O0")
target_link_libraries(assembly)
