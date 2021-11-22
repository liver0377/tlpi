#include<stdio.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int pfd1[2];
	int pfd2[2];
	char buf[1024];
	int readNum;
	int writeNum;
	
	if(argc != 2 || strcmp("--help", argv[1]) == 0)
		usageErr("%s string", argv[0]);
	if(pipe(pfd1) == -1)
		errExit("pipe");
	if(pipe(pfd2) == -1)
		errExit("pipe");
	switch(fork()){
		case -1:
			errExit("fork");
		case 0:
			if(close(pfd1[1]) == -1)
				errExit("close");
			if(close(pfd2[0]) == -1)
				errExit("close");
			//从管道中读取数据
			readNum = read(pfd1[0], buf, 1024);
			if(readNum == -1)
				errExit("read");
			for(int i = 0; i < readNum; i++)	 
				buf[i] -= 32;
			//将数据通过管道2发送给父进程
			sleep(1);				//等待父进程阻塞
			writeNum = write(pfd2[1], buf, readNum);
			if(writeNum == -1)
				errExit("write");
		 	break;
		default:
			if(close(pfd1[0]) == -1)
				errExit("close");
			if(close(pfd2[1]) == -1)
				errExit("close");
			sleep(1);		//等待子进程做好read准备
			writeNum = write(pfd1[1], argv[1], strlen(argv[1]));
			if(writeNum == -1)
				errExit("write");
			readNum = read(pfd2[0], buf, 1024);
			if(readNum == -1)
				errExit("readNum");
			printf("%s\n", buf);
			break;
	}
}
			
