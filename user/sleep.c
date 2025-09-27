#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int i = argc;

    if(i < 2) {fprintf(2, "error, please input the argument (int)\n");exit(1);}
    if(i > 2) {fprintf(2, "error, too much argument \n");exit(1);}
    

    sleep(atoi(argv[1]));

    exit(0);
}