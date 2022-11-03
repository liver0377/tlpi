#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"   
#include "../lib/tlpi_hdr.h"

static int sigCnt[NSIG];        
static volatile sig_atomic_t gotSigint = 0;
                                       

static void
handler(int sig)
{
    if (sig == SIGINT)
        gotSigint = 1;
    else
        sigCnt[sig]++;
}

int
main(int argc, char *argv[])
{
    int n, numSecs;
    sigset_t pendingMask, blockingMask, emptyMask;
    Boolean isIgnored = 0;
    struct sigaction saction;
    struct sigaction oldact;
    
    if(argc >= 2 && getopt(argc, argv, "i::") != -1) isIgnored = 1;
    saction.sa_handler = isIgnored ? SIG_IGN : handler;
    if(sigemptyset(&saction.sa_mask) == -1) errExit("sigemptyset");
    saction.sa_flags = 0;
    saction.sa_restorer = NULL;



    printf("%s: PID is %ld\n", argv[0], (long) getpid());
    if(isIgnored == 1) 
	    printf("I will ignored all the signals(except for SIGINT\n");
    for (n = 1; n < NSIG; n++)
	if(n != 2)
	   sigaction(n, &saction, &oldact); 

    if (argc > 1) {
        numSecs = atoi(argv[1]);

        sigfillset(&blockingMask);
        if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
            errExit("sigprocmask");

        printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
        sleep(numSecs);

        if (sigpending(&pendingMask) == -1)
            errExit("sigpending");

        printf("%s: pending signals are: \n", argv[0]);
        printSigset(stdout, "\t\t", &pendingMask);

        sigemptyset(&emptyMask);        
        if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
            errExit("sigprocmask");
    }

    while (!gotSigint)                  
        continue;

    for (n = 1; n < NSIG; n++)          
        if (sigCnt[n] != 0)
            printf("%s: signal %d caught %d time%s\n", argv[0], n,
                    sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

    exit(EXIT_SUCCESS);
}
