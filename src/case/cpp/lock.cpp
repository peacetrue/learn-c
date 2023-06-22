/*
LOCK 前缀是一种用于将指令转换成原子性操作的 IA-32 和 Intel 64 处理器指令前缀。当处理器执行带有 LOCK 前缀的指令时，会发出 LOCK# 信号，此信号保证当前处理器在访问共享内存时具有独占权，其他处理器需要等待 LOCK# 信号解除后才能访问这块共享内存区域，从而保证操作的原子性和一致性。

在大多数 IA-32 和所有 Intel 64 处理器中，即使没有显式使用 LOCK 前缀，也可以实现类似的原子性操作。这是因为内部硬件机制通常会自动检测并保持对共享内存的独占访问权限，从而避免数据竞争和其他线程安全问题。

LOCK 前缀只能用于以下一组指令以及对应的内存目的操作数形式：ADD、ADC、AND、BTC、BTR、BTS、CMPXCHG、CMPXCH8B、CMPXCHG16B、DEC、INC、NEG、NOT、OR、SBB、SUB、XOR、XADD 和 XCHG。如果将 LOCK 前缀用于其中的一个指令且源操作数是存储器操作数，则可能会产生未定义的操作码异常（\#UD）。如果将 LOCK 前缀用于不在该列表中的任何指令，则也会产生未定义的操作码异常。除此之外，无论是否使用 LOCK 前缀，XCHG 指令总是会发出 LOCK# 信号。

LOCK 前缀通常与 BTS 指令一起使用，用于在共享内存环境中执行读取修改写入（read-modify-write）操作。由于 LOCK 前缀和硬件机制保证了内存访问的原子性和一致性，因此可以确保多线程或多进程环境下的数据访问顺序正确。

需要注意的是，LOCK 前缀的操作与内存区域的对齐方式无关，即可以对任意不对齐的内存区域进行操作。此外，LOCK 前缀的操作在 IA-32 和 Intel 64 模式下具有相同的行为。

从P6系列处理器开始，当 LOCK 前缀被添加到一条指令上，同时该指令要访问的内存区域已经被处理器的内部高速缓存缓存时，通常不会发出 LOCK# 信号。取而代之的是，只有处理器的内部缓存被锁定。在这种情况下，处理器的高速缓存一致性机制会确保操作在内存方面是原子性的。在《Intel® 64 和 IA-32 Architectures 软件开发手册》第3A卷第9章的“锁定操作对内部处理器缓存的影响”一节中可获得更多关于缓存锁定的信息。
 */

// https://patchwork.kernel.org/project/xen-devel/patch/20200921130423.8035-1-andrew.cooper3@citrix.com/[x86: 对 smp_mb() 使用 LOCK ADD 而不是 MFENCE^]


int main() {
    asm volatile("lock":: : "memory", "cc");
    asm volatile("lock addl $0,0(%%rsp)":: : "memory", "cc");
    asm volatile("lock; addl $0,0(%%rsp)":: : "memory", "cc");
}
