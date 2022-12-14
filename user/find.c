#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
    // static char buf[DIRSIZ+1];
    char *p;

  // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;

}

void find(char *path, char *target){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    // case T_FILE:
    //     printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    //     break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            // printf("de.name = %s \n", de.name);
            if(strcmp(de.name, ".")!=0 && strcmp(de.name, "..")){
                // printf("de.name = %s de.inum = %d\n", de.name, de.inum);
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                char *name = fmtname(buf);
                if(strcmp(name, target) == 0){
                    printf("%s \n", buf);
                }
                find(buf, target);
            }
            
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]){
//   int i;

//   if(argc < 2){
//     find(".");
//     exit(0);
//   }
//   for(i=1; i<argc; i++)
//     find(argv[i]);

    find(argv[1], argv[2]);
    exit(0);
}