// 动态符号解析
#include <iostream>
#include <dlfcn.h>

int main(int argc, char **argv) {
    // 打开动态链接库
    char *workingDir = getenv("workingDir");
    const char *so_rel_path = "/learn-c/src/elf/build/libmini.so";
    char so_path[strlen(workingDir) + strlen(so_rel_path) + 1];
    strcpy(so_path, workingDir);
    strcat(so_path, so_rel_path);
    printf("so_path: %s\n", so_path);

    void *libmini = dlopen(so_path, RTLD_LAZY);
    if (!libmini) {
        std::cerr << "无法打开动态链接库: " << dlerror() << std::endl;
        return 1;
    }

    // 查找符号并获取地址
    typedef int (*Printf)(const char *fmt, ...);
    // (reinterpret_cast<func_type>(value))
    Printf printf = reinterpret_cast<Printf>(dlsym(libmini, "printf"));
    if (!printf) {
        std::cerr << "无法解析符号: " << dlerror() << std::endl;
        dlclose(libmini);
        return 1;
    }

    // 调用符号
    printf("hello %i", 10);

    // 关闭动态链接库
    dlclose(libmini);

    return 0;
}
