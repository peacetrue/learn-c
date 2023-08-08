#include <cstdio>
#include <thread>
int x = 0;
int y = 0;

void thread1() {
    x = 1;
    __sync_synchronize();  // Acquire Barrier
    printf("y = %d\n", y);
}

void thread2() {
    y = 2;
    __sync_synchronize();  // Acquire Barrier
    printf("x = %d\n", x);
}

int main() {
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.join();
    t2.join();
    return 0;
}
