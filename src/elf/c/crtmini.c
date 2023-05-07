/** 自定义的 crt 库  */
extern int main();

/** 退出进程 */
void exit(int exit_code) {
    __asm("movl %0,%%ebx \n\t"
          "movl $1,%%eax \n\t"
          "int $0x80     \n\t"
          "hlt           \n\t"
            :
            :"m"(exit_code));
}

/** ld 默认的入口 */
void _start() {
    exit(main());
}

