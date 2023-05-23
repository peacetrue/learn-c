/** 测试基础语法 */

#include "gtest/gtest.h"

static void local_static_usage() {
    static int i = 0;
    if (i < 10) {
        i++;
        local_static_usage();
        return;
    }
    ASSERT_EQ(10, i);
}

/** 测试方法内部静态变量。 */
TEST(SYNTAX, LOCAL_STATIC) {
    local_static_usage();
}

template<typename T>
static T template_max(T a, T b) {
    return a > b ? a : b;
}

/** 测试模板（泛型）。 */
TEST(SYNTAX, TEMPLATE) {
    int a = 1, b = 2;
    ASSERT_EQ(b, template_max<int>(a, b));
    double x = 3.4, y = 2.1;
    ASSERT_EQ(x, template_max<double>(x, y));
}


/** 联合体 */
TEST(SYNTAX, UNION) {
    union quantity {
        short count;
        float weight;
        float volume;
    };
    union quantity q{};
    ASSERT_EQ(NULL, q.count);
    ASSERT_EQ(NULL, q.weight);
    ASSERT_EQ(NULL, q.volume);
    q.count = 4;
    ASSERT_EQ(4, q.count);
    ASSERT_NE(NULL, q.weight);
    ASSERT_NE(NULL, q.volume);
    q = {.weight=5};
    ASSERT_EQ(5, q.weight);
    ASSERT_NE(4, q.count);
    ASSERT_NE(NULL, q.volume);
}
