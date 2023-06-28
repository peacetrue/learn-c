/** 枚举测试。 */
#include "gtest/gtest.h"

enum Color {
    Yellow,
    Purple = 2,
    Orange
};

/** 测试常规枚举 */
TEST(Enum, normal) {
    ASSERT_EQ(0, Color::Yellow);
    ASSERT_EQ(2, Color::Purple);
    ASSERT_EQ(Color::Purple + 1, Color::Orange);
}

enum class ColorClass {
    Red,
    Green = 2,
    Blue
};

/** 测试常规枚举 */
TEST(Enum, clazz) {
    printf("ColorClass::Red: %d", ColorClass::Red);
//    ASSERT_EQ(0, ColorClass::Red); // error
//    ASSERT_EQ(2, ColorClass::Green);
//    ASSERT_EQ(ColorClass::Green, ColorClass::Blue);
}
