#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char* argv[]) {
    int stat;
    char readbuf[33];// read 的缓冲区
    char pcom[MAXARG * 5] = { 0 };// 参数数组，会分成多个字符串
    char* pargs[MAXARG] = { 0 };// 指向参数数组的参数字符串
    int argcnt = 0;// 记录参数个数
    int apcnt = 0;//
    int pcnt = 0;
    int readn = 0;
    for (int i = 0; i < argc - 1; ++i, ++argcnt) {
        pargs[argcnt] = argv[i + 1];
        // printf("pargs: %s\n", pargs[argcnt]);
    }

    while ((readn = read(0, readbuf, 32)) > 0) { //0: stdin
        readbuf[readn] = 0;
        // printf("readbuf: %s", readbuf);
        for (int i = 0; i < readn; i++) {
            switch (readbuf[i]) {
            case '\n': {// exec
                pcom[pcnt] = '\0';
                pargs[argcnt] = pcom + apcnt;
                // printf("pargs: %s\n", pargs[argcnt]);
                pcnt++;
                apcnt = pcnt;
                argcnt++;

                pargs[argcnt] = 0;
                if (fork() == 0) {
                    //printf("child\n");
                    exec(argv[1], pargs);
                }
                wait(&stat);
                // 执行完后恢复到初始化
                argcnt = argc - 1;
                apcnt = 0;
                pcnt = 0;
            } break;
            case ' ': {// split
                pcom[pcnt] = '\0';// 将字符串用'\0'分段
                pargs[argcnt] = pcom + apcnt;//然后再用一个字符指针数组依次指向分段的字符串
                //printf("pargs: %s\n", pargs[argcnt]);
                pcnt++;
                apcnt = pcnt;
                argcnt++;
            } break;
            default: {// read
                pcom[pcnt] = readbuf[i];
                pcnt++;
            } break;
            }
        }

    }
    exit(0);
}