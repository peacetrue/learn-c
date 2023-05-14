/**
 * 测试可变参数相关内容。
 */

#include "gtest/gtest.h"
#include "va_usage.h"

TEST(VA, SUM) {
    // 可变参数仅在参数寄存器上
    EXPECT_EQ(va_sum_gcc(0, 0, 2, 1, 2), va_sum_custom(0, 0, 2, 1, 2));
    // 可变参数同时在参数寄存器和栈上
    EXPECT_EQ(va_sum_gcc(0, 0, 8L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L),
              va_sum_custom(0, 0, 8L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L));
    // 可变参数仅在栈上
    EXPECT_EQ(va_sum_gcc7(0, 0, 0, 0, 0, 0, 0, 2L, 1L, 2L),
              va_sum_custom7(0, 0, 0, 0, 0, 0, 0, 2L, 1L, 2L));
    EXPECT_EQ(va_sum_gcc7(0, 0, 0, 0, 0, 0, 0, 8L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L),
              va_sum_custom7(0, 0, 0, 0, 0, 0, 0, 8L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L));
}
