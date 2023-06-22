#include "stdio.h"
#include <pthread.h>
#include <unistd.h>

// 编译器优化之寄存器缓存
#ifndef VOLATILE
#define VOLATILE
#endif

VOLATILE int
        a0 = 0, b0 = 0, c0 = 0, d0 = 0, e0 = 0, f0 = 0, g0 = 0, h0 = 0, i0 = 0, j0 = 0, k0 = 0, l0 = 0, m0 = 0, n0 = 0,
        a1 = 0, b1 = 0, c1 = 0, d1 = 0, e1 = 0, f1 = 0, g1 = 0, h1 = 0, i1 = 0, j1 = 0, k1 = 0, l1 = 0, m1 = 0, n1 = 0,
        a2 = 0, b2 = 0, c2 = 0, d2 = 0, e2 = 0, f2 = 0, g2 = 0, h2 = 0, i2 = 0, j2 = 0, k2 = 0, l2 = 0, m2 = 0, n2 = 0;

static void *change(void *arg) {
    *((int *) arg) = 1;
    return arg;
}

void run(int retry_count) {
    //启动一个线程，修改 a0 = 1
    pthread_t thread;
    pthread_create(&thread, nullptr, change, (int *) &a0);
    while (true) {
        a1 = a0 + 1;
        b1 = b0 + 1;
        c1 = c0 + 1;
        d1 = d0 + 1;
        e1 = e0 + 1;
        f1 = f0 + 1;
        g1 = g0 + 1;
        h1 = h0 + 1;
        i1 = i0 + 1;
        j1 = j0 + 1;
        k1 = k0 + 1;
        l1 = l0 + 1;
        m1 = m0 + 1;
        n1 = n0 + 1;

        a2 = a0 + 1;
        b2 = b0 + 1;
        c2 = c0 + 1;
        d2 = d0 + 1;
        e2 = e0 + 1;
        f2 = f0 + 1;
        g2 = g0 + 1;
        h2 = h0 + 1;
        i2 = i0 + 1;
        j2 = j0 + 1;
        k2 = k0 + 1;
        l2 = l0 + 1;
        m2 = m0 + 1;
        n2 = n0 + 1;
        if (a1 != a2) {
            // a1 总是会去内存取最新值，如果 a2~a4 使用 a1 在寄存器的缓存值，则 a1~a4 的值总是相等的
            // 如果 a1!=a4，则 a2~a4 也是去内存取最新值，且读取到了 thread 修改的新值
            printf("matched -> a1=%i, a2=%i\n", a1, a2);
            return;
        } else if (a1 == 2) {
            // 如果 a1 读取到了最新值，此处尝试失败，必须找到一次 thread 正好在 a2~a4 间修改了值。
            a0 = 0, a1 = 0, a2 = 0;
            printf("retry: %i\n", ++retry_count);
            run(retry_count);
            return;
        }
    }
}

int main() {
    run(0);
}
