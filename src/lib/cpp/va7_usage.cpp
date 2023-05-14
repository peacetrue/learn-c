/** 可变参数 */
#include <cstdarg>
#include <cstdio>
#include "va_usage.h"

//@formatter:off
long va_sum_gcc7(long padding1, long padding2, long padding3, long padding4, long padding5, long padding6, long padding7, long count, ...) {
    long sum = 0;
    va_list ap;
    va_start(ap, count);
    while (count--) {
        long arg = va_arg(ap, long);
        sum += arg;
    }
    va_end(ap);
    return sum;
}

long va_sum_custom7(long padding1, long padding2, long padding3, long padding4, long padding5, long padding6, long padding7, long count, ...) {
    //将寄存器上的参数转移到栈上，否则难以通过指针 ++ 访问数据
    int fa_count = 8;//固定参数数目，fixed argument count
    int va_count_at_reg = A_R_C - fa_count;//寄存器上可变参数的数目，variable argument count at register
    long reg_args[A_R_C];
    if (va_count_at_reg > 0) {
        __asm  ("movq %%rdi, 0(%0)\n"
                "movq %%rsi, 8(%0)\n"
                "movq %%rdx, 16(%0)\n"
                "movq %%rcx, 24(%0)\n"
                "movq %%r8,  32(%0)\n"
                "movq %%r9,  40(%0)\n"
                :
                :"r"(reg_args)
                );
    }

    long sum = 0;
    //va_list ap;
    struct va_list_tag va_list_tags[1];// va_copy()
    struct va_list_tag ap = va_list_tags[0];// argument pointer

    //va_start(ap, count);
    ap.reg_save_area = reg_args;//寄存器参数转储在栈上的位置
    __asm ("leaq 16(%%rbp),%%rax\nmovq %%rax,%0\n":"=m"(ap.overflow_arg_area)::"rax");//栈上参数存储位置
    ap.gp_offset = (va_count_at_reg > 0 ? fa_count : -va_count_at_reg) * sizeof(long);
    ap.fp_offset = ap.gp_offset * 2;
    while (count--) {
        //long value = va_arg(ap, long);
        long value;
        if (va_count_at_reg > 0) {
            value = *(long *) ((char *) ap.reg_save_area + ap.gp_offset);
            va_count_at_reg--;
            ap.gp_offset = va_count_at_reg == 0 ? 0 : ap.gp_offset + sizeof(long);
        } else {
            value = *(long *) ((char *) ap.overflow_arg_area + ap.gp_offset);
            ap.gp_offset += sizeof(long);
        }
        //sum += value;
        sum += value;
    }
    return sum;
}
