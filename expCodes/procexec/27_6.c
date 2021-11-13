#include<signal.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"


void handler(int sig){
	printf("handler被调用\n");
}
int main(int argc, char **argv){
	struct sigaction sa;
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	
	if(sigaction(SIGCHLD, &sa, NULL) == -1) errExit("sigaction");
	if(sigprocmask(SIG_BLOCK, &set, NULL) == -1) errExit("sigprocmask");

	switch(fork()){
		case -1:
			errExit("fork");
		case 0:
			_exit(EXIT_SUCCESS);
		default:
			if(waitpid(-1, NULL, 0 > 0))
				break;
	}

	//父进程接触对SIGCHLD信号的阻塞
	if(sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) errExit("sigprocmask");
	_exit(EXIT_SUCCESS);
}
	

