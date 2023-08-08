/*内存屏障*/
// https://zhuanlan.zhihu.com/p/41872203
#ifndef FENCE_TYPE
#define FENCE_TYPE none
#endif
#define FENCE_TYPE_COMPILER compiler
#define FENCE_TYPE_PROCESSOR processor

/**无屏障*/
inline void none_fence() {
}

/**编译器屏障。只在编译期生效，目的是防止 compiler 生成乱序的内存访问指令。*/
inline void compiler_fence() {
    asm volatile("":: :"memory");
}

/**处理器屏障，内存屏障指令*/
inline void processor_fence_mfence() {
    asm volatile("mfence":: : "memory");
}

/**处理器屏障，锁前缀指令-add*/
inline void processor_fence_lock_add() {
    asm volatile("lock addl $0,0(%%rsp)":: : "memory", "cc");
}

/**处理器屏障，锁前缀指令-xchg*/
inline void processor_fence_lock_xchg(int& reg, int& mem) {
    asm volatile("xchgl %0, %1" : "+r"(reg), "+m"(mem)::"memory", "cc"); // swap(x, tmp)
}

inline void fence() {
#if defined(FENCE_COMPILER)
    compiler_fence();
#elif defined(FENCE_PROCESSOR)
    processor_fence();
#endif
}

/**全局变量声明*/
int a = 0, b = 0, r1 = 0, r2 = 0, r3 = 0;

/**
 * 寄存器缓存。
 * 导致的问题：如果有线程在 r2 = b 期间修改了 a，则 r3 无法读取到 a 的最新值。
 * 该代码不会发生重排序，因为有多个空闲的寄存器可用。
 */
void register_cache() {
    r1 = a;
    r2 = b;
    fence();
    r3 = a;
}

int
        a0 = 0, b0 = 0, c0 = 0, d0 = 0, e0 = 0, f0 = 0, g0 = 0, h0 = 0, i0 = 0, j0 = 0, k0 = 0, l0 = 0, m0 = 0, n0 = 0, o0 = 0, p0 = 0, q0 = 0,
        a1 = 0, b1 = 0, c1 = 0, d1 = 0, e1 = 0, f1 = 0, g1 = 0, h1 = 0, i1 = 0, j1 = 0, k1 = 0, l1 = 0, m1 = 0, n1 = 0, o1 = 0, p1 = 0, q1 = 0,
        a2 = 0, b2 = 0, c2 = 0, d2 = 0, e2 = 0, f2 = 0, g2 = 0, h2 = 0, i2 = 0, j2 = 0, k2 = 0, l2 = 0, m2 = 0, n2 = 0, o2 = 0, p2 = 0, q2 = 0;

/** 指令重排。*/
void reorder() {
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
//    o1 = o0 + 1;
//    p1 = p0 + 1;
//    q1 = q0 + 1;
    fence();
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
//    o2 = o0 + 1;
//    p2 = p0 + 1;
//    q2 = q0 + 1;
}

int main() {
    __sync_synchronize();
}

