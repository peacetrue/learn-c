#include "gtest/gtest.h"
#include <cstdio>
// Linux 系统调用约定：https://cq674350529.github.io/2020/01/13/C与汇编语言混合使用/

// 通常，函数调用都有一定的开销，因为函数的调用过程包括建立调用、传递参数、跳转到函数代码并返回。
// 使用宏使代码内联，可以避免这样的开销。
// 聚合多个方法的内容到一个方法中，内联函数无法在调试器中显示。
// 内联函数定义/原型
inline static void hello() {
    printf("Hello world!\n");
}

// inline 关键字，使用 INLINE
TEST(INLINE, function) {
    hello();
}

// https://akaedu.github.io/book/ch19s05.html
// http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html#s3
TEST(INLINE, ASM) {
    int a = 10, b = 0;
    EXPECT_NE(a, b);
    __asm__("movl %1, %%eax\n\t" // %1 对应 a，把 a 对应寄存器的值移动到 eax 寄存器
            "movl %%eax, %0\n\t" // %0 对应 b，把 eax 寄存器的值移动到 b 对应的寄存器
            :"=r"(b)        /* output，把寄存器的值输出到 b */
            :"r"(a)         /* input，把 a 的值保存到一个寄存器中，哪个寄存器由编译器决定 */
            :"%eax"         /* clobbered register，用到的寄存器 */
            );
    EXPECT_EQ(a, b);
}

/**
 * 调用 write 系统调用实现打印字符串。
 * @param message 字符串
 * @return 字符长度
 */
static ssize_t write_(int fd, const void *buf, size_t size) {
    ssize_t ret;
    asm(
            "movq $4, %%rax\n"  // 系统调用编号为 1，表示 write
            "movl %1, %%ebx\n"  // 第一个参数是文件描述符
            "movq %2, %%rcx\n"  // 第二个参数是要写入的数据的地址
            "movq %3, %%rdx\n"  // 第三个参数是要写入的数据的长度
            "int $0x80\n"       // 执行系统调用
            "movq %%rax, %0\n"  // 将系统调用返回值保存到 ret 变量中
            : "=r" (ret)
            : "r" (fd), "r" (buf), "r" (size)
            : "%rax", "%ebx", "%rcx", "%rdx"
            );
    return ret;
}

TEST(INLINE, WRITE) {
    const char *message = "Hello, world!\n";
    size_t len = strlen(message);
    write_(STDOUT_FILENO, message, len);
}



//该代码片段中使用了 GCC 扩展语法的内联汇编，通过 volatile 关键字和空的汇编指令来实现对内存屏障(Memory Barrier)的插入。
// 具体来说，该汇编语句会告诉编译器在此处插入一条内存屏障指令，以防止编译器进行代码重排优化，保证代码执行顺序的正确性。
TEST(INLINE, Barrier) {
    // A compiler barrier, forcing the C++ compiler to invalidate all memory assumptions
    __asm__ volatile ("" : : : "memory");
}

