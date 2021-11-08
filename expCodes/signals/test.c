#define _BSD_SOURCE
#include<signal.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

void handler(int sig){
	printf("捕获信号(%d): %s\n", sig, strsignal(sig));
}
int main(int argc, char **argv){
	struct sigaction sa;
	sigset_t fullset, emptyset;
	siginfo_t info;

	if(sigemptyset(&emptyset) == -1) errExit("sigemptyset");
	if(sigfillset(&fullset) == -1) errExit("sigfillset");

	sa.sa_handler = handler;
	sa.sa_flags = 0;

        if(sigdelset(&fullset, SIGKILL) == -1) errExit("sigdelset");
	if(sigdelset(&fullset, SIGSTOP) == -1) errExit("sigdelset");
	if(sigprocmask(SIG_BLOCK, &fullset, NULL) == -1) errExit("sigprocmask");

	//为所有信号设置阻塞
	for(int i = 1; i < NSIG; i++)
		if(i != SIGKILL && i != SIGTERM)
		if(sigaction(i, &sa, NULL) == -1)
			errExit("sigaction");

	printf("请使用kill -... %d向该进程发送各种信号\n, 有30秒钟时间", getpid());
	sleep(30);

	printf("解除阻塞");
	
	for(;;){
		if(sigwaitinfo(&fullset, &info) == -1) errExit("sigwaitinfo");
		sleep(1);
	}

	return 0;

}

