#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	pid_t childPid;
	int status;
	char buf[1024];

	int fd = open("/tmp/test.txt", O_RDWR | O_SYNC | O_CREAT, O_RDWR | S_IRUSR | S_IWUSR);
	if(fd == -1) errExit("open");
	if(write(fd, "hello world\n", 13) == -1) errExit("write");

	switch(childPid = vfork()){
		case -1:
			errExit("fork");
		case 0:
			if(close(fd) == -1) errExit("close");
			_exit(EXIT_SUCCESS);
			break;
		default:
			lseek(fd, 0, SEEK_SET);
			if(read(fd, buf, 13) == -1) errExit("read");
			if(write(STDOUT_FILENO, buf, 13) == -1) errExit("read");
			break;
	}

	exit(EXIT_SUCCESS);
}	
			

