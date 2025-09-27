#include "kernel/types.h"
#include "user/user.h"

// recursion
void
prime_sieve(int p_left) {
    int prime;
    int p_right[2];
    int n;

    // 从左侧管道读取第一个素数
    if (read(p_left, &prime, 4) == 0) {
        // 如果管道为空，说明没有更多的素数了
        close(p_left);
        exit(0);
    }

    // 打印当前素数
    printf("prime %d\n", prime);

    // 为下一个素数创建新的管道
    if (pipe(p_right) == -1) {
        fprintf(2, "pipe failed\n");
        close(p_left);
        exit(1);
    }
    
    // 创建一个子进程，用于下一个素数筛
    if (fork() == 0) {
        // 这是子进程
        close(p_left);
        close(p_right[1]); // 关闭右侧管道的写端
        prime_sieve(p_right[0]);
    } else {
        // 这是父进程
        close(p_right[0]); // 关闭右侧管道的读端
        
        // 循环读取左侧管道的数字
        while (read(p_left, &n, 4) > 0) {
            // 如果数字不能被当前素数整除，则写入右侧管道
            if (n % prime != 0) {
                write(p_right[1], &n, 4);
            }
        }
        
        // 数据传输完毕，关闭管道
        close(p_left);
        close(p_right[1]);
        
        // 等待所有子进程结束
        wait(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    int i;
    
    // 创建第一个管道
    if (pipe(p) == -1) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    
    // 创建第一个子进程
    if (fork() == 0) {
        // 这是子进程
        close(p[1]); // 关闭写端
        prime_sieve(p[0]);
    } else {
        // 这是父进程
        close(p[0]); // 关闭读端
        
        // 将数字 2 到 35 写入管道
        for (i = 2; i <= 35; i++) {
            write(p[1], &i, 4);
        }
        
        // 关闭写端，向子进程发送 EOF 信号
        close(p[1]);
        
        // 等待所有子进程结束
        wait(0);
    }

    exit(0);
}
