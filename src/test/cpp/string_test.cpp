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

TEST(String, strcpy) {
    const char *src = "peace-";
    char dst[strlen(src) + 1];
    strcpy(dst, "peace");
    EXPECT_FALSE(strcmp("peace", dst));
    strcpy(dst, "-");
    EXPECT_FALSE(strcmp("-", dst));
}


TEST(String, strcat) {
    const char *src = "peace-";
    char dst[strlen(src) + 1];
    dst[0] = 0;  // 确保目标字符串为空字符串
    strcat(dst, "peace");
    strcat(dst, "-");
    EXPECT_FALSE(strcmp(src, dst));
}

TEST(String, strstr) {
    const char *str = "123";
    const char *substr = "2";
    const char *result = strstr(str, substr);
    printf("result: %s\n", result);
    EXPECT_FALSE(strcmp("23", result));

    size_t front = strlen(str) - strlen(result);
    printf("front: %zu\n", front);
    char dst[front + 1];
    dst[front] = 0;
    EXPECT_FALSE(strcmp("1", strncpy(dst, str, front)));
}
