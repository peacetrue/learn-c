#include <pthread.h>
#include <cstdio>

static void *thread_func(void *arg) {
    return arg;
}

/**
 * 返回输入的参数。
 */
void *pthread_create_usage(void *arg) {
    pthread_t thread;
    pthread_create(&thread, nullptr, thread_func, arg);
    void *thread_ret_val;
    pthread_join(thread, &thread_ret_val);
    return thread_ret_val;
}

