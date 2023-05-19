#ifdef __linux__
#include "gtest/gtest.h"
#include <pthread.h>
#include <unistd.h>

// 在一写多读的场景下，缓存一致性协议能够保证数据的可见性。
// 写线程修改值，读线程可以读取到写线程改动后的值。
// 要求编译器不对代码进行任何优化。
// 运行指定测试：src/test/test --gtest_filter=CACHE_COHERENCE_LINUX*
#define REPEAT 100
#define NEW_VALUE (-1)

static void *change_local(void *arg) {
    printf("2. x change to %i on core: %d\n", NEW_VALUE, sched_getcpu());
    *((int *) arg) = NEW_VALUE;
    return arg;
}

TEST(CACHE_COHERENCE_LINUX, LOCAL) {
    for (int i = 0; i < REPEAT; ++i) {
        int x = i;
        printf("1. x=%i on core: %d\n", x, sched_getcpu());

        pthread_t thread;
        cpu_set_t cpuset;
        int cpuCore = i % sysconf(_SC_NPROCESSORS_ONLN);  // 设置线程运行在第 n 个核心上（编号从 0 开始）
        pthread_create(&thread, nullptr, change_local, &x);
        // 设置线程的亲和性
        CPU_ZERO(&cpuset);
        CPU_SET(cpuCore, &cpuset);
        pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

        while (x != NEW_VALUE);//忙等
        printf("3. x=%i\n\n", x);
        EXPECT_EQ(NEW_VALUE, x);
    }
}

static int x = 0;

static void *change_global(void *arg) {
    printf("2. x change to %i on core: %d\n", NEW_VALUE, sched_getcpu());
    x = NEW_VALUE;
    return arg;
}

TEST(CACHE_COHERENCE_LINUX, GLOBAL) {
    for (int i = 0; i < REPEAT; ++i) {
        x = 0;
        printf("1. x=%i on core: %d\n", x, sched_getcpu());

        cpu_set_t cpuset;
        int cpuCore = i % sysconf(_SC_NPROCESSORS_ONLN);  // 设置线程运行在第 n 个核心上（编号从 0 开始）
        pthread_t thread;
        pthread_create(&thread, nullptr, change_global, &x);
        // 设置线程的亲和性
        CPU_ZERO(&cpuset);
        CPU_SET(cpuCore, &cpuset);
        pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

        while (x != NEW_VALUE);//忙等
        printf("3. x=%i\n\n", x);
        EXPECT_EQ(NEW_VALUE, x);
    }
}

#endif
