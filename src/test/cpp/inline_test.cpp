#include "gtest/gtest.h"
#include <cstdio>

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

//该代码片段中使用了 GCC 扩展语法的内联汇编，通过 volatile 关键字和空的汇编指令来实现对内存屏障(Memory Barrier)的插入。
// 具体来说，该汇编语句会告诉编译器在此处插入一条内存屏障指令，以防止编译器进行代码重排优化，保证代码执行顺序的正确性。
TEST(INLINE, Barrier) {
    // A compiler barrier, forcing the C++ compiler to invalidate all memory assumptions
    __asm__ volatile ("" : : : "memory");
}

