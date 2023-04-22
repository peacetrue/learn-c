# 导入 Google Test 框架
if (NOT googletest_POPULATED) # 如果已经拉取过了，不重复拉取
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.13.0
    )
    FetchContent_Populate(googletest)
    add_subdirectory(
            ${googletest_SOURCE_DIR}
            ${googletest_BINARY_DIR}
    )
endif ()
#set()
# 配置测试模块
include_directories(../lib/public) # 指定头文件目录
aux_source_directory(cpp TEST_SRCS) # 指定源文件目录
add_executable(test ${TEST_SRCS})
target_link_libraries(test gtest gtest_main lib)
