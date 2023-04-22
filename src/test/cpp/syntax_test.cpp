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
