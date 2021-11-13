#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "../lib/tlpi_hdr.h"

//父进程先创建了一个子进程，随后又创建了一个孙进程，其中子进程在创建完成后立即退出，而孙进程则要稍微延时一会
//因此，子进程比孙进程先退出，那么孙进程随即就会被init进程所收养，并且由init自动调用wait
//使用这种方法，意在使用一个孙进程来替代子进程，并且不会产生僵尸进程
//由于子进程很快就会退出，所以父进程在执行完wait调用回收子进程后就能去做自己的事了，而不用去管孙进程
int main(int argc, const char *argv[]){
    
    int status;
    int childPid = fork();

    if(childPid == -1)
        errExit("fork1");
    if(childPid == 0){
        switch(fork()){
            case -1: errExit("fork2");
            case  0: 
                /* Do something important */
                exit(EXIT_SUCCESS);
            default:
                exit(EXIT_SUCCESS);
        }
    }

    /* wait for it */
    if(waitpid(childPid, &status, 0) == -1)
        errExit("waitpid");
    return 0;
}
