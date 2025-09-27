#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[])
{
    // === 第 1 步: 准备基础命令 ===
    char *command = argv[1];   //比较规范的写法 
    char *new_argv[MAXARG];    //param.h
    int base_argc = argc - 1;
    for(int i = 0; i < base_argc; i++){
        new_argv[i] = argv[i+1];
    }

    // === 第 2 步: 循环读取标准输入 ===
    char line_buf[512];
    int p = 0;
    char c;

    while(read(0, &c, 1) == 1) {    //逐字
        if (c == '\n') {
            line_buf[p] = '\0';

            // === 第 3 步: 组合参数并执行 ===

            if (fork() == 0) {
                new_argv[base_argc] = line_buf;
                new_argv[base_argc + 1] = 0;
                exec(command, new_argv);
                fprintf(2, "xargs: exec %s failed\n", command);
                exit(1);
            }
            wait(0);

            p = 0; // 为下一行做准备
        } else {
            line_buf[p++] = c;
        }
    }

    exit(0);
}