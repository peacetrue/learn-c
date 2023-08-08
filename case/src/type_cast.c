#include <stdio.h>

/**
 * 类型转换，long 转换为 int，溢出值如何处理。
 */
int main() {
    long var_long = 0x0123456789ABCDEF;
    int var_int = (int) var_long;
    printf("long: %lxu\nint: %xu\n", var_long, var_int);
}




