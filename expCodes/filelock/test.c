#include <sys/file.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int fd;
	fd = open("tfile", O_RDONLY);
	if (fd == -1)
		errExit("open");

	
	//加互斥锁
	if (flock(fd, LOCK_EX | LOCK_NB) == -1)
		errExit("flock: ex");

	//加共享锁
	if (flock(fd, LOCK_SH) == -1)
		errExit("flock: sh");

	if (flock(fd, LOCK_UN) == -1)
		errExit("flock");
	return 0;
}

