/**
 *  测试 ELF 文件相关内容。
 */

#include "gtest/gtest.h"

/**
 * "_ZN"是一个标识符前缀，表示这是一个C++符号；
 * "3"表示下一个名称部分的长度；"elf"是名称的实际部分，表示类名或命名空间名称；
 * "3"又表示下一个名称部分的长度；"var"是名称的另一部分，可能是类的数据成员、静态变量或命名空间中的变量名；
 * "E"表示名称的结束。
 * c++filt _ZN3elf3varE
 */
extern "C" int _ZN3elf3varE;

namespace elf {
    int var = 42;
}

/** 符号修饰 */
TEST(ELF, DECORATE_SYMBOL) {
    EXPECT_EQ(42, _ZN3elf3varE);
    elf::var = 1;
    EXPECT_EQ(1, _ZN3elf3varE);
}
