#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"../lib/tlpi_hdr.h"

char buf[1]={'a'};
int main(int argc, char** argv){
	int fd;
	int numWrite;

	//以O_APPEND标志打开文件
	if(argc == 3){
		fd = open(argv[1], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | 
				S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if(fd == -1)
			errExit("open");
		for(int i = 0;i < atoi(argv[2]); i++){
			numWrite = write(fd, buf, 1);	//每次写入一个字节
			if(numWrite == -1)
				errExit("write");
		}
		exit(EXIT_SUCCESS);
	}

	if(argc == 4){
		fd = open(argv[1], O_CREAT | O_WRONLY,  S_IRUSR | S_IWUSR |
                                S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

		if(fd == -1)
			errExit("open");
		for(int i = 0;i < atoi(argv[2]);i++){
			if(lseek(fd, 0, SEEK_END) == -1)
					errExit("lseek");
			numWrite = write(fd, buf, 1);
			if(numWrite == -1)
				errExit("write");
		}

		exit(EXIT_SUCCESS);

	}
}
