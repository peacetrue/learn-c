#include <stdio.h>

void test_init() __attribute__((section(".init"))) /*__attribute__ ((noreturn))*/;

void test_fini() __attribute__((section(".fini"))) /*__attribute__ ((noreturn))*/;

void test_init() {
    printf("test_init!\n");
}

void test_fini() {
    printf("test_fini!\n");
}
