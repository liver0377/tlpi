#include<setjmp.h>
#include<signal.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

static sigjmp_buf senv;

void handler(int sig)
{
    printf("In the handler. But I do nothing\n");
}

void m_abort(void){
	//首先将SIGABRT信号从当前mask中移除
	sigset_t set;
	if(sigemptyset(&set) == -1) errExit("sigemptyset");
	if(sigaddset(&set, SIGABRT) == -1) errExit("sigaddset");
	if(sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) errExit("sigprocmask");
	
	printf("m_abort: raise first abort\n");	
	//向进程自身发送SIG_ABRT信号
	raise(SIGABRT);

	//如果进程仍未结束,将当前对SGIABRT信号的处置设为默认，再次发送SIGABRT信号
	struct sigaction sa;
	if(sigemptyset(&sa.sa_mask) == -1) errExit("sigempty");
	sa.sa_flags = SA_RESETHAND;				//使用该标志后，会在调用处理器函数之前将信号处置重置为默认值，即SIG_DEF(core)

	if(sigaction(SIGABRT, &sa, NULL) == -1)  errExit("sigaction");
	printf("m_abort: raisr second SIGABORT\n");
	//再次向自身发送SIGABRT信号
	raise(SIGABRT);
}


int main(void)
{
    printf("Now call the abort.\n");
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    sigaction(SIGABRT, &sa, NULL);

    m_abort();
}
