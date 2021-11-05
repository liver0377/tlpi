#include<sys/stat.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"
#include<stdio.h>


int dup(int fd){
	int newfd;
	
	newfd = fcntl(fd, F_DUPFD, fd+1);
	if(newfd == -1)
		errExit("fcntl");
	return newfd;
}


int main(int argc, char **argv){
	int oldfd;
	int newfd;
	oldfd = open("tfile", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if(oldfd == -1)
		errExit("open");

	printf("tfile\n");
	printf("the old file descriptor is %d \n", oldfd);

	newfd = dup(oldfd);
	printf("the new file descriptor is %d \n", newfd);
	exit(EXIT_SUCCESS);
}
