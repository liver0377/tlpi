#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char **argv) {
        int childPid;
        childPid = fork();
        int fd1, fd2;
        switch (childPid) {
                case -1:
                        errExit("fork");
                case 0:
                        fd1 = open("tfile1", O_RDONLY);
                        if (fd1 == -1)
                                errExit("open");
                        if (flock(fd1, LOCK_EX) == -1)
                               errExit("flock");
                        sleep(1);   //等待其父进程先发出锁请求

                        fd2 = open("tfile2", O_RDONLY);
                        if (fd2 == -1)
                                errExit("open");

                        if (flock(fd2, LOCK_EX) == -1) {
                                if (errno == EDEADLK) {
                                        printf("child: deadlock!\n");
                                        exit(-1);
                                } else {
                                        printf("child: some unkonwn error\n");
                                        exit(-1);
                                }
                                                                            }
                        } else {
                                printf("child exit\n");
                        }
                        break;
                default:
                        fd1 = open("tfile2", O_RDONLY);
                        if (fd1 == -1)
                                errExit("open");
                        if (flock(fd1, LOCK_EX) == -1)
                                errExit("flock");

                        fd2 = open("tfile1", O_RDONLY);
                        if (fd2 == -1)
                                errExit("open");

                        if (flock(fd2, LOCK_EX) == -1) {
                                if (errno == EDEADLK) {
                                        printf("parent: deadlock!\n");
                                        exit(-1);
                                } else {
                                        printf("parent: some unknown error \n");
                                        exit(-1);
                                }
                        } else {
                                printf("parent exit\n");
                        }
                        break;
        }

        return 0;
}
