#include "gtest/gtest.h"

TEST(String, literal) {
    // string 字面量是一个存储在只读代码段的常量
    const char *name_ptr = "peace";
    //name_ptr[0] = 1; //不能修改字面量的内容
    // Process finished with exit code 138 (interrupted by signal 10: SIGBUS)
}

TEST(String, strcmp) {
    char name[] = {'p', 'e', 'a', 'c', 'e', 0};
    EXPECT_EQ(0, strcmp(name, "peace"));
}
