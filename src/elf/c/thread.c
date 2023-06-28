/** 多线程的例子。观察一个进程内有多个线程。 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static void *thread_func2(void *arg) {
    printf("peer handle_thread 2: %lu\n", pthread_self());
    sleep(10);
    return arg;
}

static void *thread_func(void *arg) {
    printf("peer handle_thread: %lu\n", pthread_self());

    pthread_t thread;
    pthread_create(&thread, NULL, thread_func2, arg);
    void *thread_ret_val;
    pthread_join(thread, &thread_ret_val);

    return arg;
}

int main(int argc, char **argv) {
    printf("main handle_thread: %lu\n", pthread_self());

    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, &argc);
    void *thread_ret_val;
    pthread_join(thread, &thread_ret_val);
}
