#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include"../lib/tlpi_hdr.h"

#define BUF_SIZE 1024
char buf[BUF_SIZE];

int main(int argc, char ** argv){
	int fd = open("tfile", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(fd == -1)
		errExit("open");
	int offset = 10;
	printf("ready to read\n");
	int numRead = read(STDIN_FILENO, buf, BUF_SIZE);		//将标准输入读取进buf缓冲区
	if(numRead == -1)
		errExit("read");
	int numPwrite = pwrite(fd, buf, numRead, offset);
	if(numPwrite == -1)
		errExit("pwrite");
	int numPread = pread(fd, buf, numRead, offset);
	if(numPread == -1)
		errExit("pread");
	printf("tfile[10:] : %s\n",buf);
	if(close(fd)==-1)
		errExit("close");
	return 0;
}
	
	


