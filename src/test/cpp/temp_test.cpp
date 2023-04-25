/** 临时测试。 */
#include "gtest/gtest.h"

#define LEN 3
int array[LEN] = {1, 2, 3};

/** 测试 array sizeof：编译时运算结果 */
TEST(TEMP, ARRAY_SIZEOF) {
    EXPECT_EQ(LEN * sizeof array[0], sizeof array);
}
