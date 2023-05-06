/** 演示不需要依赖 crt 和 libc */
char *str = "Hello world!\n";

/**
 * int write(int filedesc, char* buffer, int size);
 * WRITE 调用的调用号为 4，则 eax = 0。 filedesc 表示被写入的文件句柄，使用ebx寄存器传递，我们这里是要往
 * 默认终端(stdout)输出，它的文件句柄为0，即ebx = 0。 buffer表示要写入的缓冲区地址，使用ecx寄存器传递，我们这里要输出
 * 字符串str，所以ecx = str。
 * size 表示要写入的字节数，使用edx寄存器传递，字符串“Hello
 * world!\n”长度为13字节，所以edx = 13。
 */
void print() {
    __asm("movl $13,%%edx \n\t"
          "movq %0,%%rcx  \n\t"
          "movl $0,%%ebx  \n\t"
          "movl $4,%%eax  \n\t"
          "int $0x80      \n\t"
            ::"r"(str):"edx", "ecx", "ebx");
}

/**
 * EXIT 系统调用中，ebx 表示进程退出码(Exit Code)。
 */
void exit() {
    __asm("movl $42,%ebx  \n\t"
          "movl $1,%eax  \n\t"
          "int $0x80\n\t");
}

void nomain() {
    print();
    exit();
}
