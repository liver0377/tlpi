#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"   
#include "../lib/tlpi_hdr.h"

static int sigCnt[NSIG];        
static volatile sig_atomic_t gotSigint = 0;
                                       
void showmask(sigset_t mask){
	printf("the mask is:\n");
	for(int i = 1; i < NSIG; i++){
		if(sigismember(&mask, i) == 1)
			printf("%s(%d)\n", strsignal(i), i);
	}
}


static void
handler(int sig)
{
    if (sig == SIGINT)
        gotSigint = 1;
    else
        sigCnt[sig]++;
    printf("I caught the signal(%d):  %s\n!", sig, strsignal(sig));
}

int
main(int argc, char *argv[])
{
    int n, numSecs;
    struct sigaction saction;
    struct sigaction oldact;
    int opt;

   while((opt = getopt(argc, argv, "r::n::")) != -1){
	  switch(opt){
		  case 'r': saction.sa_flags |= SA_RESETHAND;    break;
		  case 'n': saction.sa_flags |= SA_NODEFER;      break;
		  case '?': printf("invalid option\n");
	  }
   }

    
    saction.sa_handler = handler;
    if(sigemptyset(&saction.sa_mask) == -1) errExit("sigemptyset");
    saction.sa_restorer = NULL;



    printf("%s: PID is %ld\n", argv[0], (long) getpid());

    for (n = 1; n < NSIG; n++)
	   sigaction(n, &saction, &oldact); 

    if (argc > 1) {
        numSecs = atoi(argv[optind]);

        printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
        sleep(numSecs);

	showmask(saction.sa_mask);
    }


    for (n = 1; n < NSIG; n++)          
        if (sigCnt[n] != 0)
            printf("%s: signal %d caught %d time%s\n", argv[0], n,
                    sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

    exit(EXIT_SUCCESS);
}
