#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "../../lib/tlpi_hdr.h"

//2.只有当所有的文件描述符副本都被关闭之后文件锁才会被释放
int main(int argc, char **argv) {
        int fd;
        fd = open("tfile", O_RDONLY);
        if (fd == -1)
                errExit("open");

        //加锁
        if (flock(fd, LOCK_EX) == -1)
                errExit("flock");
        int childPid = fork();
        if (childPid == -1)
                errExit("fork");
        if (childPid > 0) {
                close(fd);   //关闭自身文件描述符，此时只有子进程拥有文件描述符副本
                wait(NULL);  //等待子进程终止
                exit(0);
        }
        sleep(1);

        //子进程有一份文件描述符副本,此时文件还未解锁，子进程对文件的加锁会失败
        int newfd = open("tfile", O_RDONLY);
        if (newfd == -1)
                errExit("open");
        if (flock(newfd, LOCK_EX | LOCK_NB) == -1 && errno == EWOULDBLOCK) {
                printf("child try lock failed!\n");
        }
        //子进程关闭从父进程复制而来的文件描述符副本
        close(fd);

        if (flock(newfd, LOCK_EX | LOCK_NB) != -1){
                printf("you are right!\n");
                exit(0);
        }
}
