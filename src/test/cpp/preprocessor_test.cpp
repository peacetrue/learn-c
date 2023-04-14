#include "gtest/gtest.h"
// https://wangdoc.com/clang/preprocessor

// 预处理器，宏定义模板替换。

TEST(Preprocessor, variable) {
#define NUM 100
    EXPECT_EQ(100, NUM);

#define HELLO "Hello, world"
    EXPECT_EQ("Hello, world", HELLO);
}

TEST(Preprocessor, function) {
#define SQUARE(X) X*X
    EXPECT_EQ(4, SQUARE(2));

#define MAX(x, y) ((x)>(y)?(x):(y))
    EXPECT_EQ(4, MAX(3 + 2, 4 + 2));
}

TEST(Preprocessor, concat) {

#define CONCAT_STR(X) #X"X"#X
    EXPECT_EQ("1X1", CONCAT_STR(1));

#define CONCAT_VAR(X1, X2) X1##X2
    EXPECT_EQ(11, CONCAT_VAR(1, 1));
}

