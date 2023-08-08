/** 睡眠案例，执行时阻塞，可方便获取运行时内存映射。*/

#include <unistd.h>
#include <limits.h>

int main() {
    sleep(INT_MAX);
}
