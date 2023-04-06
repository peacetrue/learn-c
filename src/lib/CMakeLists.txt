include_directories(public) # 指定头文件目录
aux_source_directory(cpp LIB_SRCS) # 指定源文件目录
add_library(lib ${LIB_SRCS})
