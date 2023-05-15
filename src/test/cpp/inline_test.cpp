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

/*
在 Linux 系统中，`syscall` 和 `int 0x80` 都是用于调用系统调用的指令，但它们在实现上存在一些区别。

`int 0x80` 是一种传统的系统调用方式，
 在早期的 Linux 系统中广泛使用。当执行 `int 0x80` 指令时，
 处理器会进入内核态并跳转到系统调用处理程序，
 由该处理程序根据传入的系统调用号来调用对应的系统调用函数。
 在 `int 0x80` 指令中，系统调用号会被传递到 `eax` 寄存器中，
 而其他的参数则会被传递到其他的寄存器中，例如文件描述符、指向缓冲区的指针等。

`syscall` 是一种较新的系统调用方式，
 它是在 Linux 2.5 版本中引入的。与 `int 0x80` 不同，
 `syscall` 使用固定的寄存器来传递系统调用号和其他参数，
 具体来说，系统调用号会被传递到 `rax` 寄存器中，
 而其他的参数则会被传递到 `rdi`、`rsi`、`rdx`、`r10`、`r8` 和 `r9` 等寄存器中，
 这些寄存器被称为系统调用参数寄存器。

相比于 `int 0x80`，`syscall` 具有以下优点：

1. `syscall` 的参数传递更加灵活，可以传递更多的参数，并且不需要使用堆栈来传递参数，避免了栈操作的开销。
2. `syscall` 的处理程序使用了更加高效的方式来处理系统调用，可以避免一些不必要的操作，提高了系统调用的效率。
3. `syscall` 支持更多的系统调用，包括一些新的系统调用，例如 `ioctl` 等。
需要注意的是，使用 `syscall` 指令时需要满足一些条件，例如必须在 64 位系统中使用，
而且需要使用指定的寄存器来传递系统调用号和参数。
因此，在编写系统调用相关的代码时，需要根据具体的系统和处理器架构选择合适的指令。
*/


/**
 * write 系统调用。
 * @param fd 文件描述符
 * @param buf 输出缓冲区
 * @param count 缓冲区字节长度
 * @return 输出内容字节长度
 */
long write_(long fd, const void *buf, long count) {
    long ret = -1;
    __asm  (
            "movq $1, %%rax\n\t"  // write 系统调用号为 1
            "movq %1, %%rdi\n\t"  // 文件描述符 stdout = 1
            "movq %2, %%rsi\n\t"  // 字符串地址
            "movq %3, %%rdx\n\t"  // 字符串长度
            "syscall\n\t"
            "movq %%rax, %0\n\t"
            :"=r"(ret)
            : "r"(fd), "r"(buf), "r"(count)
            : "rax", "rdx", "rdi", "rsi"
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

