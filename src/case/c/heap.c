#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

long resolveArg(int argc, char **argv, int index, long defaults) {
    return argc >= index + 1 ? strtol(argv[index], NULL, 10) : defaults;
}

/**
 * 观察堆内存。
 *
 * @param argc 参数数目
 * @param argv 参数指针。
 *  <p/> argv[0]：程序路径
 *  <p/> argv[1]：内存分配方式，默认为 1。 1：mmap，2：brk，3：malloc
 *  <p/> argv[2]：内存分配空间，默认为 0，不分配空间
 *  <p/> argv[3]：睡眠时长（秒），默认 1 秒。程序驻留在运行中，以便观察内存映射
 *  @return 退出码
 */
int main(int argc, char **argv) {
    long size = resolveArg(argc, argv, 1, 0);
    long way = resolveArg(argc, argv, 2, 1);
    long seconds = resolveArg(argc, argv, 3, 1);
    printf("size: %ld, way: %ld, seconds: %ld\n", size, way, seconds);
    if (size == 0) {
        if (seconds > 0) sleep(seconds);
        return 0;
    }

    size *= 1024 * 1024; //内存空间 M
    void *buffer;
    if (way == 1) {
        buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    } else if (way == 2) {
        buffer = sbrk((int) size);
    } else if (way == 3) {
        buffer = malloc(size);
    } else {
        exit(1);
    }
    printf("buffer location: %p\n", buffer);
    if (seconds > 0) sleep(seconds);
}
