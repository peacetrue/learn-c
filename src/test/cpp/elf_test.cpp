/**
 *  测试 ELF 文件相关内容。
 */

#include <csetjmp>
#include "gtest/gtest.h"

/**
 * "_Z"是一个标识符前缀，表示这是一个C++符号；N 表示含命名空间；
 * "3"表示下一个名称部分的长度；"elf"是名称的实际部分，表示类名或命名空间名称；
 * "3"又表示下一个名称部分的长度；"var"是名称的另一部分，可能是类的数据成员、静态变量或命名空间中的变量名；
 * "E"表示名称的结束。
 * c++filt _ZN3elf3varE
 */
extern "C" int _ZN3elf3varE;

namespace elf {
    int var = 42;
}

/** 符号修饰 */
TEST(ELF, DECORATE_SYMBOL) {
    EXPECT_EQ(42, _ZN3elf3varE);
    elf::var = 1;
    EXPECT_EQ(1, _ZN3elf3varE);
}

/* 测试执行顺序 */

void constructor_init() __attribute__((constructor));

void constructor_init() {
    printf("constructor_init!\n");
}

int static_init() {
    printf("static_init!\n");
    return 0;
}

// 必须有返回值，不能直接调用 static_init()
int g = static_init();

void atexit_fini() {
    printf("atexit_fini!\n");
}

/** 测试 atexit */
TEST(ELF, ATEXIT) {
    printf("main!\n");
    atexit(&atexit_fini);
    //constructor_init!
    //static_init!
    //main!
    //atexit_fini!
    //destructor_fini
}

void destructor_fini() __attribute__((destructor));

void destructor_fini() {
    printf("destructor_fini!\n");
}



/**
 * printf 的狂乱输出。
 * 在这个程序里，printf的第一个输出参数是一个 int(4字节)，
 * 而我们告诉printf它是一个double(8字节以上)，
 * 因此printf的输出会错误，由于printf在读取double的时候实际造成了越界，
 * 因此后面几个参数的输出也会失败。
 */
TEST(ELF, PRINTF) {
    printf("%i\t%d\t%c\n", 1, 666, 'a');
    printf("%lf\t%d\t%c\n", 1, 666, 'a');
}

/*
 * 非局部跳转即使在 C 语言里也是一个备受争议的机制。
 * 使用非局部跳转，可以实现从一个函数体内向另一个事先登记过的函数体内跳转，而不用担心堆栈混乱。
 * 实际上，当 setjmp 正常返回的时候，会返回 0，因此会打印出"Hello "的字样。
 * 而 longjmp 的作用，就是让程序的执行流回到当初 setjmp 返回的时刻，并且返回由 longjmp 指定的返回值( longjmp 的参数 2)，
 * 也就是 1，自然接着会打印出“World!”并退出。
 * 换句话说，longjmp 可以让程序"时光倒流"回 setjmp 返回的时刻，并改变其行为，以至于改变了未来。
 * 是的，这绝对不是结构化编程
 */
jmp_buf point;

TEST(ELF, LONG_JMP) {
    if (setjmp(point)) {
        printf("World!");
    } else {
        printf("Hello ");
        longjmp(point, 1);
    }
    // Hello World!
}
