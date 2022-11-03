//#define _LARGEFILE64_SOURCE 
#include<sys/stat.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"

int main(int argc,char** argv){
	int  fd;
	off_t off;
	if(argc != 3 || argv[1] == "--help" ){
		usageErr("%s pathname offset\n", argv[0]);
	}

	fd = open(argv[1], O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);

	if(fd == -1)
		errExit("open64");

	
	off = atoi(argv[2]);

	if(lseek(fd, off, SEEK_SET) == -1)
		errExit("lseek64");

	if(write(fd, "test", 4) == -1)
		errExit("write");

	exit(EXIT_SUCCESS);
}
