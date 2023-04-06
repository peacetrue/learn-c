#include <cstdio>
#include "pthread_usage.h"

int main() {
    void *thread_ret_val = pthread_create_usage((char *) "Hello World!");
    printf("thread_ret_val: %s\n", (char *) thread_ret_val);
}
