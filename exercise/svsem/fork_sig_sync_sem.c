#include"semun.h"
#include "../time/curr_time.h"                  /* Declaration of currTime() */
#include "../lib/tlpi_hdr.h"


int
main(int argc, char *argv[])
{
    pid_t childPid;

    setbuf(stdout, NULL);               /* Disable buffering of stdout */

    int semid;
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT);
    if (semid == -1)
        errExit("semid");
    union semun arg;
    arg.val = 0;

    if (semctl(semid, 0, SETVAL, arg) == -1)
        errExit("semctl");

    struct sembuf sops[1];
    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0: /* Child */

        /* Child does some required action here... */

        printf("[%s %ld] Child started - doing some work\n",
	               currTime("%T"), (long) getpid());
        sleep(2);               /* Simulate time spent doing some work */


        printf("[%s %ld] Child about to increse the sempahore\n",
                currTime("%T"), (long) getpid());

        /* Now child can do other things... */
        sops[0].sem_num = 0;
        sops[0].sem_op = 1;
        sops[0].sem_flg = 0;

        if (semop(semid, sops, 1) == -1)
            errExit("semop");

        _exit(EXIT_SUCCESS);

    default: /* Parent */

        /* Parent may do some work here, and then waits for child to
           complete the required action */

        printf("[%s %ld] Parent about to wait for signal\n",
                currTime("%T"), (long) getpid());
        sops[0].sem_num = 0;
        sops[0].sem_op = -1;
        sops[0].sem_flg = 0;

        if (semop(semid, sops, 1) == -1)
           errExit("semop");
        printf("[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

        exit(EXIT_SUCCESS);
    }
}
     	
                                                                                                                                                                        1,9           Top
