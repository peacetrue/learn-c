/** 生命周期演示案例 */
#include <stdio.h>
#include <stdlib.h>

void test_preinit_array() {
    printf("test_preinit_array!\n");
}

__attribute__((section(".preinit_array"))) void (*test_preinit_array_ptr)(void) = test_preinit_array;

//void init() __attribute__((section(".init"))); .init 段中拼接代码必须通过汇编实现，链接 init_fini.o

void test_init_array() __attribute__((constructor));
//__attribute__((section(".init_array"))) void (*test_init_array_ptr)(void) = test_init_array;

void test_init_array() {
    printf("test_init_array!\n");
}

void test_fini_array() __attribute__((destructor));
//__attribute__((section(".fini_array"))) void (*test_destructor_init_ptr)(void) = test_init_array;

void test_fini_array() {
    printf("test_fini_array!\n");
}

/** 此测试函数必须有返回值 */
int test_invoke_init() {
    printf("test_invoke_init!\n");
    return 0;
}

int useless = test_invoke_init();// 此语句需 C++ 支持，否则 error: initializer element is not constant
//test_invoke_init(); //错误，不支持无返回值调用

void test_atexit_fini() {
    printf("test_atexit_fini!\n");
}

int main() {
    atexit(&test_atexit_fini);
    printf("test_main!\n");
    return 0;
}
