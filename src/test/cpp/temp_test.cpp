/** 临时测试。 */
#include "gtest/gtest.h"

#define LEN 3
int array[LEN] = {1, 2, 3};

/** 测试 array sizeof：编译时运算结果 */
TEST(TEMP, ARRAY_SIZEOF) {
    EXPECT_EQ(LEN * sizeof array[0], sizeof array);
    printf("bool: %lu\n", sizeof(bool));
    printf("char: %lu\n", sizeof(char));
    printf("short: %lu\n", sizeof(short));
    printf("int: %lu\n", sizeof(int));
    printf("long: %lu\n", sizeof(long));
    printf("float: %lu\n", sizeof(float));
    printf("double: %lu\n", sizeof(double));
    printf("ptr: %lu\n", sizeof(void *));
}


/** 测试 array sizeof：编译时运算结果 */
TEST(TEMP, ARRAY_SIZEOF1) {
    // C 语言强制类型转换，long 转换为 int，溢出值如何处理
    unsigned long a = 0xffffffffffffffff;
    printf("long: %lu\n", a);
    unsigned int b = (int) a;
    printf("long: %u\n", b);
}
