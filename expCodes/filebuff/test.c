#include<unistd.h>
#include"../lib/tlpi_hdr.h"
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, char **argv){
	int fd;
	fd = open("tail.c", O_RDONLY, O_RDONLY);
	if(fd == -1)
		errExit("open");
	
	close(fd);
	return 0;
}
