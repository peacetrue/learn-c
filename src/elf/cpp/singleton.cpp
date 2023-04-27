/** 独立案例，用于分析变量在汇编及 ELF 中的表示 */
#include <cstdio>

int init_var_cross_file = 1; // 初始化 变量 跨文件共享
int unin_var_cross_file;     // 未初始 变量 跨文件共享
static int init_var_in_file = 2; // 初始化 变量 当前文件共享
static int unin_var_in_file; // 未初始 变量 当前文件共享

void print(int num) {
    printf("sum=%d\n", num);
}

int main() {
    static int init_var_cross_current_method = 3; // 初始化 变量 跨当前方法共享
    static int unin_var_cross_current_method;     // 未初始 变量 跨当前方法共享

    int init_var_in_method = 4; // 初始化 变量 当前方法共享
    int unin_var_in_method;     // 未初始 变量 当前方法共享

    {
        int init_var_in_block = 5;  // 初始化 变量 块内共享
        int unin_var_in_block;      // 未初始 变量 块内共享
    }

    print(
            init_var_cross_file
            + unin_var_cross_file
            + init_var_in_file
            + unin_var_in_file
            + init_var_cross_current_method
            + unin_var_cross_current_method
            + init_var_in_method
            + unin_var_in_method
    );

    return 0;
}
