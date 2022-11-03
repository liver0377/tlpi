#include<unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	pid_t childPid;
	int status;

	switch(childPid = fork()){
		case -1:
			errExit("fork");
		case 0:
			sleep(1);
			exit(-1);
		default:
			wait(&status);
			printf("the status of child is:%d\n", WEXITSTATUS(status));
	}
	exit(EXIT_SUCCESS);
}
		
