#include<stdio.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	pid_t childPid;

	switch(childPid = fork()){
		case -1:
			errExit("fork");
		case 0:
			sleep(3);	//等待其父进程终止
			printf("childPid = %ld, parentPid = %ld\n", (long)getpid(), (long)getppid());
			_exit(EXIT_SUCCESS);
		default:
			_exit(EXIT_SUCCESS);
	}
}
