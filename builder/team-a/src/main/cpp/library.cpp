#include "stdio.h"
#include "library.h"

/**
 * 私有方法，仅供内部调用
 */
void private_method() {
    printf("private_method\n");
}

void public_method() {
    printf("public_method\n");
    private_method();
}
