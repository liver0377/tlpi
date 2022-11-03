#include "lib/tlpi_hdr.h"
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("output.txt", O_RDONLY);
    if (fd == -1)
        errExit("open");

    int mode = fcntl(fd, F_GETFD) & O_ACCMODE;
    if (mode == O_RDONLY) {
        printf("O_RDONLY\n");
    } else if (mode == O_WRONLY) {
        printf("O_WRONLYL\n");
    } else if (mode == O_WRONLY) {
        printf("O_WRONLY\n");
    }
}