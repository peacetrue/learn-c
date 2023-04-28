add_executable(empty cpp/empty.cpp)
add_executable(singleton cpp/singleton.cpp)
add_executable(sleep cpp/sleep.cpp)

add_library(group_lib cpp/group_lib.cpp cpp/group_lib.h)
add_executable(group_main_debug cpp/group_main_debug.cpp cpp/group_lib.h)
target_link_libraries(group_main_debug group_lib)
add_executable(group_main_info cpp/group_main_info.cpp cpp/group_lib.h)
target_link_libraries(group_main_info group_lib)
