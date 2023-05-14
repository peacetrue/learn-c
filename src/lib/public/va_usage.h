struct va_list_tag {
    unsigned int gp_offset;
    unsigned int fp_offset;
    void *overflow_arg_area;
    void *reg_save_area;
};

#define A_R_C 6 //用于传递参数的寄存器数目，argument register count

/**
 * 累加可变参数值。
 * @param count 参数数目
 * @param ... 可变参数
 * @return 和
 */
long va_sum_gcc(long padding1, long padding2,
                long count, ...);

/**
 * 累加可变参数值。
 * @param count 参数数目
 * @param ... 可变参数
 * @return 和
 */
long va_sum_custom(long padding1, long padding2,
                   long count, ...);


/**
 * 累加可变参数值。固定参数耗尽参数寄存器，可变参数全部在栈区。
 * @param count 参数数目
 * @param ... 可变参数
 * @return 和
 */
long
va_sum_gcc7(long padding1, long padding2, long padding3, long padding4, long padding5, long padding6, long padding7,
            long count, ...);

/**
 * 累加可变参数值。固定参数耗尽参数寄存器，可变参数全部在栈区。
 * @param count 参数数目
 * @param ... 可变参数
 * @return 和
 */
long
va_sum_custom7(long padding1, long padding2, long padding3, long padding4, long padding5, long padding6, long padding7,
               long count, ...);
