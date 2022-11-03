#include <signal.h>
#include <libgen.h>    
#include "../lib/tlpi_hdr.h"

#define CMD_SIZE 200

int
main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;

    sigset_t set;
    sigaddset(&set, SIGCHLD);

    if(sigprocmask(SIG_BLOCK, &set, NULL) == -1) errExit("sigprocmask");  	//为父进程设置信号掩码，以防父进程在子进程退出之前未能够调用sigwaitinfo
    setbuf(stdout, NULL);       /* Disable buffering of stdout */

    printf("Parent PID=%ld\n", (long) getpid());

    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0:     /* Child: immediately exits to become zombie */
        printf("Child (PID=%ld) exiting\n", (long) getpid());
        _exit(EXIT_SUCCESS);

    default:    /* Parent */

	sigwaitinfo(&set, NULL);		//使用sigwaitinfo来使得父进程绝对在子进程退出后开始执行后面的语句
        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
        system(cmd);            /* View zombie child */

        /* Now send the "sure kill" signal to the zombie */

        if (kill(childPid, SIGKILL) == -1)
            errMsg("kill");
        sleep(3);               /* Give child a chance to react to signal */
        printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
        system(cmd);            /* View zombie child again */

        exit(EXIT_SUCCESS);
    }
}
