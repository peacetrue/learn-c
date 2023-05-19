#include "gtest/gtest.h"
#include <pthread.h>

// 在一写多读的场景下，缓存一致性协议能够保证数据的可见性。
// 写线程修改值，读线程可以读取到写线程改动后的值。
// 要求编译器不对代码进行任何优化。
#define REPEAT 100
#define NEW_VALUE (-1)

static void *change_local(void *arg) {
    printf("2. x change to %i\n", NEW_VALUE);
    *((int *) arg) = NEW_VALUE;
    return arg;
}

TEST(CACHE_COHERENCE, LOCAL) {
    for (int i = 0; i < REPEAT; ++i) {
        int x = i;
        printf("1. x=%i\n", x);
        pthread_t thread;
        pthread_create(&thread, nullptr, change_local, &x);
        while (x != NEW_VALUE);//忙等
        printf("3. x=%i\n\n", x);
        EXPECT_EQ(NEW_VALUE, x);
    }
}

static int x = 0;

static void *change_global(void *arg) {
    printf("2. x change to %i\n", NEW_VALUE);
    x = NEW_VALUE;
    return arg;
}

TEST(CACHE_COHERENCE, GLOBAL) {
    for (int i = 0; i < REPEAT; ++i) {
        x = i;
        printf("1. x=%i\n", x);
        pthread_t thread;
        pthread_create(&thread, nullptr, change_global, &x);
        while (x != NEW_VALUE);//忙等
        printf("3. x=%i\n\n", x);
        EXPECT_EQ(NEW_VALUE, x);
    }
}
