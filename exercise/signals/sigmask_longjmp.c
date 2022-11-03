#define _GNU_SOURCE 
#include<string.h>
#include<setjmp.h>
#include<signal.h>
#include"signal_functions.h"
#include"../lib/tlpi_hdr.h"

static volatile sig_atomic_t canJump = 0;	//表示是否能够起跳

//使用USE_SIGSETJMP宏来标识是使用setjmp还是sigsetjmp
#ifdef USE_SIGSETJMP
static sigjmp_buf env;
#else
static jmp_buf env;
#endif

static void 
handler(int sig){
	printf("Received signal %d (%s), signal mask is: \n", sig,strsignal(sig));
       
	printSigMask(stdout, NULL);

	if(!canJump){
	    printf("'env buffer not yet set, doing a simple return \n");
	    return ;
	}

#ifdef USE_SIGSETJMP
    siglongjmp(senv,1);
#else
    longjmp(env, 1);
#endif
}

int main(int argc, char **argv){
	struct sigaction sa;

	printSigMask(stdout, "signal mask at stardut:\n");
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(SIGINT, &sa, NULL) == -1)
		errExit("sigaction");

#ifdef USE_SIGSETJMP
	printf("Calling sigsetjmp()\n");
	if(sigsetjmp(senv, 1) == 0)
#else
	printf("Calling setjmp()\n");
	if(setjmp(env) == 0)
#endif
	canJump = 1;

	else
		printSigMask(stdout, "After jump from handler, signal mask is:\n");
	
	for(;;)
		pause();
}
