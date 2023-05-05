/** 实现一个自定义的 bash 程序，用于演示 bash 的大致逻辑。 */
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char buf[1024] = {0};
    pid_t pid;
    while (true) {
        printf("minibash$");
        scanf("%s", buf);
        pid = fork();
        if (pid == 0) {
            if (execlp(buf, nullptr) < 0) {
                printf("exec error\n");
            }
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            printf("fork error %d\n", pid);
        }
    }
    return 0;
}
