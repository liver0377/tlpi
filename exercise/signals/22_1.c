#include<signal.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

void handler(int sig){
	printf("捕获到了信号(%d): %s\n", sig, strsignal(sig));
}

int main(int argc, char **argv){
	struct sigaction sa;
	sigset_t set;

	if(sigemptyset(&set) == -1) errExit("sigemptyset");
	if(sigaddset(&set, SIGCONT) == -1) errExit("sigaddset");

	sa.sa_handler = handler;
	sa.sa_flags = 0;

	if(sigaction(SIGCONT, &sa, NULL) == -1) errExit("sigaction");
	if(sigprocmask(SIG_BLOCK, &set, NULL) == -1) errExit("sigprocmask");		//将SIGCONT添加进阻塞信号

	printf("SIGCONT已被添加进信号掩码\n");
	printf("进程终止，请使用 kill -18 %d 重新激活该进程\n", getpid());
	raise(19);									//自己中断自己

	printf("进程被重新激活\n");
	printf("SIGCNOT已从信号掩码中移除\n");

	if(sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) errExit("sigprocmask");
	raise(2);

	

	return 0;
}
