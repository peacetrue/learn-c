/** 分析 Linux 线程的实现原理。 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>

long resolveArg(int argc, char **argv, int index, long defaults) {
    return argc >= index + 1 ? strtol(argv[index], NULL, 10) : defaults;
}

static unsigned long gettid() {
    // Linux 下如何获取线程 id
    // https://stackoverflow.com/questions/21091000/how-to-get-thread-id-of-a-pthread-in-linux-c-program/32211287#32211287
    return syscall(SYS_gettid);
}

void *create_thread(void *(*thread_func)(void *), long count) {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, (void *) count);
    void *thread_ret_val;
    pthread_join(thread, &thread_ret_val);
    return thread_ret_val;
}

static void *create_thread_func(void *arg);


static void *create_thread_func_log(void *arg) {
    printf("peer thread id: %li\n", gettid());
    return create_thread_func(arg);
}

/**
 * 创建线程后执行的函数，此函数内部继续创建线程。
 *
 * @param arg 还需要创建的线程数。为 0 时，不再创建线程
 */
static void *create_thread_func(void *arg) {
    long count = (long) arg;
    if (count > 0) {
        return create_thread(create_thread_func_log, count - 1);
    }
    return arg;
}

/**
 * 创建多个线程，然后观察线程 id。
 *
 * @param argc 参数数目
 * @param argv 参数指针。
 *  <p/> argv[0]：程序路径
 *  <p/> argv[1]：创建的线程数目，默认为 1
 *  @return 退出码
 */
int main(int argc, char **argv) {
    printf("process id: %d\n", getpid());
    printf("main thread id: %li\n", gettid());
    create_thread_func((void *) resolveArg(argc, argv, 1, 1));
}

