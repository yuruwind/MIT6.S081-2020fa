#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2], p2[2];
    char byte = 'a';
    pipe(p1);
    pipe(p2);
    int pid = fork();

    if(!pid) {
        close(p1[1]);
        close(p2[0]);

        read(p1[0], &byte, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p2[1], &byte, 1);

    } else {
        close(p1[0]);
        close(p2[1]);

        write(p1[1], &byte, 1);
        wait(0);
        read(p2[0], &byte, 1);
        fprintf(1, "%d: received pong\n", getpid());
    }

    exit(0);
}