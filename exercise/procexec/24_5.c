#include<signal.h>
#include"../time/curr_time.h"
#include"../lib/tlpi_hdr.h"

#define SYNC_SIG SIGUSR1
#define SYNC_SIG_2 SIGUSR2

static void
handler(int sig){}

int main(int argc, char **argv){
	pid_t childPid;
	sigset_t blockMask, originMask, emptyMask; sigset_t set;
	struct sigaction sa;

	setbuf(stdout, NULL);

	sigemptyset(&blockMask);
	sigaddset(&blockMask, SYNC_SIG);
	if(sigprocmask(SIG_BLOCK, &blockMask, &originMask) == -1) 
		errExit("sigprocmask");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if(sigaction(SYNC_SIG, &sa, NULL) == -1) 
		errExit("sigaction");

	if(sigemptyset(&set) == -1) errExit("sigemptyset");
	sigaddset(&set, SYNC_SIG_2);

	switch(childPid = fork()){
		case -1:
			errExit("fork");
		case 0:
			printf("[%s %ld] Child started - doing some work\n",
					currTime("%T"), (long) getpid());

			sleep(2);

			printf("[%s %ld] Child about to signal parent\n",
					currTime("%T"), (long) getpid());
			if(sigprocmask(SIG_BLOCK, &set, NULL) == -1) errExit("sigprocmask");  //提前将SYNC_SIG_2加入掩码，以防在进入sigwaitindo之前就接受到信号
			if(kill(getppid(), SYNC_SIG) == -1)
				errExit("kill");
			
			
			if(sigwaitinfo(&set, NULL) == -1)
				errExit("sigwatiindo");
			printf("[%s %ld] child got signal \n",
					currTime("%T"), (long) getpid());

			_exit(EXIT_SUCCESS);
		default:
			printf("[%s %ld] Parent about to wait for signal\n",
					currTime("%T"), (long) getpid());
			sigemptyset(&emptyMask);
			if(sigsuspend(&emptyMask) == -1 && errno != EINTR)
				errExit("sigsuspend");

			printf("[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

			if(kill(childPid, SYNC_SIG_2) == -1) 
			       errExit("kill");	
			if(sigprocmask(SIG_SETMASK, &originMask, NULL) == -1)
				errExit("sigprocmask");

			exit(EXIT_SUCCESS);
	}
}
