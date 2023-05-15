/** 独立案例，用于分析变量在汇编及 ELF 中的表示 */
#include <stdio.h>

// 值类型
int global_init_var = 1; // 全局 初始化 变量
int global_unin_var;     // 全局 未初始 变量
static int static_init_var = 2; // 静态 初始化 变量
static int static_unin_var;     // 静态 未初始 变量
//数组类型（结构体同理），仅使用 全局 初始化 变量
char global_init_var_chars[] = "number";
const char *global_init_var_chars_ptr = &global_init_var_chars[3];
int global_init_var_ints[] = {1, 2, 3, 4, 5};
int *global_init_var_ints_ptr = &global_init_var_ints[3];

void global_print(int number) {
    printf("%s=%d\n", global_init_var_chars, number);
}

static void static_print(int number) {
    printf("%s=%d\n", global_init_var_chars_ptr, number);
}

int main() {
    static int static_init_var_m = 3; // 静态 初始化 变量 方法内
    static int static_unin_var_m;     // 静态 未初始 变量 方法内

    int local_init_var = 4; // 本地 初始化 变量
    int local_unin_var;     // 本地 未初始 变量

    int sum = global_init_var
              + global_unin_var
              + static_init_var
              + static_unin_var
              + static_init_var_m
              + static_unin_var_m
              + local_init_var
              + local_unin_var;

    global_print(sum);
    static_print(sum);
}

