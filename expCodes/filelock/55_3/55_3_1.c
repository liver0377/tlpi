#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../../lib/tlpi_hdr.h"

//1.当文件描述符指向同一个打开文件句柄时，关闭其中一个文件描述符就会关闭该文件上的文件锁
int main(int argc, char **argv) {
        int fd1, fd2;
        fd1 = open("tfile", O_RDONLY);
        if (fd1 == -1)
                errExit("fd1");
        fd2 = dup(fd1);

        //加锁
        if (flock(fd1, LOCK_EX) == -1)
                errExit("flock");

        //通过fd2解锁
        if (flock(fd2, LOCK_UN) == -1)
                errExit("flock");

        //检测是否能够再次加锁
        if (flock(fd1, LOCK_EX | LOCK_NB) != -1) {
                    printf("you are right\n");
                    exit(0);
        }

        printf("you are wrong\n");
        exit(-1);
}
