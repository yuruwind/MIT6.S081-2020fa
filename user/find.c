#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name){
    int fd;
    char *p, buf[512];
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }




        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");

        }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;

        // 将 de.name 安全地拷贝到 buf 中，p 指向这个新拷贝的名字
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;


        if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0){
            continue;
        }


    if(strcmp(name, p) == 0) printf("%s\n", buf);

        //resursion
      stat(buf, &st);
      if(st.type == T_DIR)
        find(buf, name);


  }
  close(fd);
}

int
main (int argc, char* argv[])
{
    if(argc != 3) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}