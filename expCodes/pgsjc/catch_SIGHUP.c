#define _XOPEN_SOURCE 500
#include<unistd.h>
#include<signal.h>
#include"../lib/tlpi_hdr.h"

static void 
handler(int sig){
}

int main(int argc, char **argv){
	pid_t childPid;
	struct sigaction sa;

	setbuf(stdout, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	if(sigaction(SIGHUP, &sa, NULL) == -1)
		errExit("sigaction");

	childPid = fork();
	if(childPid == -1)
		errExit("fork");

	if(childPid == 0 && argc > 1){
		if(setpgid(0, 0) == -1)			//如果提供了一个命令行参数，那么将会为子进程新创建一个进程组，子进程变为该进程组的首进程
			errExit("setpgid");
	}

	printf("PID: %ld PGID:  %ld PPID: %ld SID: %ld\n", (long) getpid(), (long) getpgrp(), (long) getppid(), (long) getsid(0));
	alarm(20);
	for(;;){
		pause();
		printf("SIGHUP is called\n");
	}

	return 0;
}

