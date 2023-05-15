/* 自定义的 crt 库  */

/**
 * 主函数。
 * @param argc 参数数目
 * @param argv 参数数组
 * @return 退出码
 */
extern int main(int argc, char *argv[]) __attribute__((weak));

/**
 * 退出进程。重命名以防止编译器 noreturn 警告。
 * @param exit_code 退出码
 */
void exit_mini(int exit_code) {
    __asm  ("movl $1,%%eax \n\t"
            "movl %0,%%ebx \n\t"
            "int $0x80     \n\t"
            "hlt           \n\t"
            :
            :"r"(exit_code)
            :"eax", "ebx"
            );
}

/** ld 默认的入口函数。*/
void _start() {
    if (!&main)exit_mini(1);
    char *rbp_ptr = 0;
    __asm("mov %%rbp,%0":"=r"(rbp_ptr));
    int exit_code = main(*(rbp_ptr + 8), (char **) (rbp_ptr + 16));
    exit_mini(exit_code);
}
