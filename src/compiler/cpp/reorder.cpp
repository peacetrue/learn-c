#include "stdio.h"
#include <pthread.h>
#include <limits.h>

// 编译器优化之指令重排
// 编写一系列语句，如果观察到后一条语句的执行结果先于前一条语句，则证明发生了指令重新排序。
// 一个线程不停地修改值，另一个线程检查是不是有序修改
#ifndef VOLATILE
#define VOLATILE
#endif

// 0 系列为初始值
// 1、2 系列为更新值，a1=a0+1; a2=a0+1
// 变量足够多，寄存器无法放下
VOLATILE int
        a0 = 0, b0 = 0, c0 = 0, d0 = 0, e0 = 0, f0 = 0, g0 = 0, h0 = 0, i0 = 0, j0 = 0, k0 = 0, l0 = 0, m0 = 0, n0 = 0, o0 = 0,p0 = 0,q0 = 0,
        a1 = 0, b1 = 0, c1 = 0, d1 = 0, e1 = 0, f1 = 0, g1 = 0, h1 = 0, i1 = 0, j1 = 0, k1 = 0, l1 = 0, m1 = 0, n1 = 0, o1 = 0,p1 = 0,q1 = 0,
        a2 = 0, b2 = 0, c2 = 0, d2 = 0, e2 = 0, f2 = 0, g2 = 0, h2 = 0, i2 = 0, j2 = 0, k2 = 0, l2 = 0, m2 = 0, n2 = 0, o2 = 0,p2 = 0,q2 = 0;

bool over = false;

void *check_reorder(void *arg) {
    for (long i = 0; i < LONG_MAX; ++i) {
        if (a2 > n1) {
            printf("a2 execute before n1: %li\n", i);
            over = true;
            break;
        }
        printf("check times: %li\n", i); //必须，否则优化为死循环
    }
    return arg;
}

void run() {
    //启动一个线程，检查值是否被有序修改
    pthread_t thread;
    pthread_create(&thread, nullptr, check_reorder, nullptr);

    //循环修改值
    for (long i = 1; i < LONG_MAX; ++i) {
        //修改值，每一轮前面的变量值先修改，都大于后面的变量值
        a1 = a0 + i;
        // a2 = a0 + i; 重新排序后，会将此条语句放在 a1 下面，以便共用 a0 所在寄存器；正常情况下，前面的变量值总大于后面的
        b1 = b0 + i;
        c1 = c0 + i;
        d1 = d0 + i;
        e1 = e0 + i;
        f1 = f0 + i;
        g1 = g0 + i;
        h1 = h0 + i;
        i1 = i0 + i;
        j1 = j0 + i;
        k1 = k0 + i;
        l1 = l0 + i;
        m1 = m0 + i;
        n1 = n0 + i;
        o1 = o0 + i;
        p1 = p0 + i;
        q1 = q0 + i;

        a2 = a0 + i;
        b2 = b0 + i;
        c2 = c0 + i;
        d2 = d0 + i;
        e2 = e0 + i;
        f2 = f0 + i;
        g2 = g0 + i;
        h2 = h0 + i;
        i2 = i0 + i;
        j2 = j0 + i;
        k2 = k0 + i;
        l2 = l0 + i;
        m2 = m0 + i;
        n2 = n0 + i;
        o2 = o0 + i;
        p2 = p0 + i;
        q2 = q0 + i;
        //如果检查到重新排序，则退出
        if (over)return;
        printf("set times: %li\n", i); //必须，否则优化为死循环
    }
}

int main() {
    run();
}
