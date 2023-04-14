#include "gtest/gtest.h"
#include <pthread.h>

// 在一写多读的场景下，缓存一致性协议能够保证数据的可见性。
// 写线程修改值，读线程可以读取到写线程改动后的值。
// 要求编译器不对代码进行任何优化。

static void *change_local(void *arg) {
    printf("2. x change to %i\n", 1);
    *((int *) arg) = 1;
    return arg;
}

TEST(CACHE_COHERENCE, LOCAL) {
    int x = 0;
    printf("1. x=%i\n", x);
    pthread_t thread;
    pthread_create(&thread, nullptr, change_local, &x);
    while (!x);//忙等
    printf("3. x=%i\n", x);
    EXPECT_EQ(1, x);
}

int x = 0;

static void *change_global(void *arg) {
    printf("2. x change to %i\n", 1);
    x = 1;
    return arg;
}

TEST(CACHE_COHERENCE, GLOBAL) {
    printf("1. x=%i\n", x);
    pthread_t thread;
    pthread_create(&thread, nullptr, change_global, &x);
    while (!x);//忙等
    printf("3. x=%i\n", x);
    EXPECT_EQ(1, x);
}
