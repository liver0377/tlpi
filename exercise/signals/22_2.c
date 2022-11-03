#include<signal.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"
void handler(int sig){
    printf("%d\n", sig);
}

int main(int argc, const char *argv[]){

    printf("NSIG=%d RTMIN=%d RTMAX=%d\n", NSIG, SIGRTMIN, SIGRTMAX);

    sigset_t st;
    sigfillset(&st);

    struct sigaction siga;
    siga.sa_handler = handler;
    sigemptyset(&st);
    siga.sa_flags = 0;

    for(int i = 1; i < NSIG; i++)
        if(i != SIGKILL && i != SIGTERM)
            if(sigaction(i, &siga, NULL) == -1)
                errno = 0;
    
    if(sigprocmask(SIG_BLOCK, &st, NULL) == -1)   errExit("sigprocmask");

    union sigval val = { .sival_int = 0 };
    for(int i = SIGRTMIN; i <= SIGRTMAX; i++ )
        sigqueue(getpid(), i, val);
    for(int i = 1; i < 32; i++)
        if(i != SIGTSTP && i != SIGABRT && i != SIGSTOP && i != SIGKILL && i != SIGTERM)
            raise(i);
    sleep(1);

    if(sigprocmask(SIG_UNBLOCK, &st, NULL) == -1) errExit("sigprocmask");

    return 0;
}
