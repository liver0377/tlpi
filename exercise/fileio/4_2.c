#include "../lib/tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char **argv) {
    int inputFd, outputFd, flag;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file \n", argv[0]);

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);

    flag = O_CREAT | O_WRONLY | O_TRUNC;
    outputFd = open(argv[2], flag);
    if (outputFd == -1)
        errExit("opening file %s", argv[2]);

    /*Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead) {
            fatal("couldn't write whole buffer");
        }
    }

    if (numRead == -1)
        errExit("read");

    exit(EXIT_SUCCESS);
}
