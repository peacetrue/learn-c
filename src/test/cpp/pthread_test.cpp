#include "gtest/gtest.h"
#include <pthread_usage.h>

TEST(pthread, pthread_create_usage) {
    char *arg = (char *) "Hello, World!";
    EXPECT_EQ(arg, pthread_create_usage(arg));
}


/*
 * 线程专有数据（Thread-Specific Data，TSD）的缩写，
 * 它是多线程编程中的一个重要概念。
 * TSD 允许程序员为每个线程关联一些私有数据，
 * 这些数据对于其他线程是不可见的。
 */
static pthread_key_t key;

void *tsd_func(void *arg) {
    auto tid = (intptr_t) arg;
    pthread_setspecific(key, (void *) tid);
    printf("Thread %ld sets TSD: %ld\n", tid, tid);
    auto tsd = (intptr_t) pthread_getspecific(key);
    printf("Thread %ld gets TSD: %ld\n", tid, tsd);
    return nullptr;
}

TEST(pthread, tsd) {
    pthread_key_create(&key, nullptr);
    pthread_t threads[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], nullptr, tsd_func, (void *) (intptr_t) i);
    }
    for (auto &thread: threads) {
        pthread_join(thread, nullptr);
    }
}


/*
 * 线程局部存储（TLS，Thread Local Storage）。
 */
static __thread int tls_flag = 0;

void *tls_func(void *arg) {
    auto tid = (intptr_t) arg;
    tls_flag = (int) tid;
    printf("Thread %ld gets TLS: %i\n", tid, tls_flag);
    return nullptr;
}

TEST(pthread, tls) {
    pthread_t threads[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], nullptr, tls_func, (void *) (intptr_t) i);
    }
    for (auto &thread: threads) {
        pthread_join(thread, nullptr);
    }

    printf("Parent Thread gets TLS: %i\n", tls_flag);
    EXPECT_EQ(0, tls_flag);
}

