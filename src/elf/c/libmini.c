//------------- 字符串 ----------------

/**
 * 求字符串长度，不包含末尾的 0。
 * @param str 字符串指针
 * @return 字符串长度
 */
static unsigned long strlen(const char *str) {
    unsigned cnt = 0;
    if (!str) return 0;
    for (; *str != '\0'; ++str) ++cnt;
    return cnt;
}

/**
 * 拷贝字符串。
 * @param dest 目的字符串
 * @param src 源字符串
 * @return 字符串长度
 */
static char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
    return ret;
}

/**
 * 将整数转换为 ascii 字符串。
 * @param integer 整数
 * @param str 字符串
 * @param radix 转换进制
 * @return 字符串，与入参 str 相同
 */
static char *itoa(int integer, char *str, int radix) {
    char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *p = str;
    char *head = str;
    if (!p || radix < 2 || radix > 36)
        return p;
    if (radix != 10 && integer < 0)
        return p;
    if (integer == 0) {
        *p++ = '0';
        *p = 0;
        return p;
    }
    if (radix == 10 && integer < 0) {
        *p++ = '-';
        integer = -integer;
    }
    while (integer) {
        *p++ = digit[integer % radix];
        integer /= radix;
    }
    *p = 0;
    for (--p; head < p; ++head, --p) {
        char temp = *head;
        *head = *p;
        *p = temp;
    }
    return str;
}

//------------- IO 操作 ----------------

/**
 * write 系统调用。
 * @param fd 文件描述符
 * @param buf 输出缓冲区
 * @param count 缓冲区字节长度
 * @return 输出内容字节长度
 */
static long write(long fd, const void *buf, long count) {
    long ret = -1;
    __asm  ("movq $1, %%rax\n\t"  // write 系统调用号为 1
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

#define STDOUT_FILENO 1

static int write_char(char c) {
    return write(STDOUT_FILENO, &c, 1);
}

static int write_chars(const char *chars) {
    return write(STDOUT_FILENO, chars, strlen(chars));
}

static int write_integer(int integer) {
    char buf[16];
    itoa(integer, buf, 10);
    return write_chars(buf);
}

#include <stdarg.h>

//@formatter:off

/**
 * 格式化打印，只支持 %s 和 %d。
 * @param fmt 格式字符串
 * @param ... 可变参数
 * @return 输出内容的字节长度
 */
int printf(const char *fmt, ...) {
    int count = 0;
    va_list args;
    va_start(args, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int n = va_arg(args, int);
                count += write_integer(n);
            } else if (*fmt == 's') {
                char *s = va_arg(args, char *);
                count += write_chars(s);
            }
        } else {
            count += write_char(*fmt);
        }
        fmt++;
    }
    va_end(args);
    return count;
}


