#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

long resolveArg(int argc, char **argv, int index, long defaults) {
    return argc >= index + 1 ? strtol(argv[index], NULL, 10) : defaults;
}

/**
 * 使用不同的方式分配虚拟内存，然后睡眠数秒，此时使用命令观察虚拟内存分布情况。
 *
 * @param argc 参数数目
 * @param argv 参数指针。
 *  <p/> argv[0]：程序路径
 *  <p/> argv[1]：内存分配方式，默认为 1。 1：brk，2：mmap，3：malloc
 *  <p/> argv[2]：内存分配次数，默认为 0
 *  <p/> argv[3]：内存分配空间，默认为 0，不分配空间
 *  <p/> argv[4]：睡眠时长（秒），默认 1 秒。程序驻留在运行中，以便观察内存映射
 *  @return 退出码
 */
int main(int argc, char **argv) {
    int index = 0;
    long way = resolveArg(argc, argv, ++index, 1);
    long count = resolveArg(argc, argv, ++index, 0);
    long size = resolveArg(argc, argv, ++index, 0);
    long seconds = resolveArg(argc, argv, ++index, 1);
    printf("way: %ld, count: %ld, size: %ld, seconds: %ld\n", way, count, size, seconds);
    if (way <= 0 || way > 3 || count <= 0 || size <= 0) {
        if (seconds > 0) sleep(seconds);
        return 0;
    }

    size *= 1024 * 1024; //内存空间单位 M
    void *buffer[count]; //内存分配的地址
    printf("buffer location: ");
    for (int i = 0; i < count; ++i) {
        if (way == 1) {
            buffer[i] = sbrk((int) size);
        } else if (way == 2) {
            buffer[i] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        } else if (way == 3) {
            buffer[i] = malloc(size);
        }
        printf("%p, ", buffer[i]);
    }
    printf("\n");
    if (seconds > 0) sleep(seconds);
}
