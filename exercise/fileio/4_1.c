#include "../lib/tlpi_hdr.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 1024

char buf[BUF_SIZE];

void tee(int flag, char *file) {
    int fd = open(file, flag);
    if (fd == -1)
        errExit("open");

    while (1) {
        int numRead = read(STDIN_FILENO, buf, BUF_SIZE);
        if (numRead == -1)
            errExit("read"); //读取失败

        int numWritten = write(STDOUT_FILENO, buf, numRead);
        if (numWritten == -1)
            errExit("wirte"); //写入失败

        numWritten = write(fd, buf, numRead);
        if (numWritten == -1)
            errExit("write"); //写入失败
    }
}

int main(int argc, char **argv) {
    char opt = '\0';
    while ((opt = getopt(argc, argv, "a:")) != -1) {
        // 有选项a的情况
        // 选项为a，在已存在的文件后追加内容
        if (argc < 3) {
            usageErr("%s  [-a file] | [file]", argv[0]);
            break;
        }
        int flag = O_CREAT | O_WRONLY | O_APPEND;
        tee(flag, argv[2]);
    }

    if (argc == 2) {
        int flag = O_CREAT | O_WRONLY | O_TRUNC;
        tee(flag, argv[1]);
    }

    return 0;
}
