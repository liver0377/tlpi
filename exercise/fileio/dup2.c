#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"
int dup2(int oldfd, int newfd){
	//首先检查old的文件描述符的正确性
	int numFcntl;
	numFcntl = fcntl(oldfd, F_GETFL);
	if(numFcntl == -1)
		errno = EBADF;
			
	if(oldfd == newfd)
		return oldfd;
	else{
		numFcntl = fcntl(oldfd, F_DUPFD, newfd);
		if(numFcntl == -1)
			errExit("fcntl");
	}
	return numFcntl;
}

int main(int argc,char** argv){
	int newfd;
	int oldfd = open("tfile", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if(oldfd == -1)
		errExit("open");
	newfd = dup2(oldfd, 5);
	if(newfd == -1)
		errExit("dup2");
	printf("tfile\n");
	printf("old file descriptor: %d\n",oldfd);
	printf("new file descriptor: %d\n",newfd);
	
	exit(EXIT_SUCCESS);
}


