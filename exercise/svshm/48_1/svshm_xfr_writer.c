/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 48-2 */

#include "svshm_xfr.h"

int main(int argc, char *argv[])
{
    int semid, shmid, bytes, xfrs;
    struct shmseg *shmp;
    union semun dummy;

    /* Create set containing two semaphores; initialize so that
       writer has first access to shared memory. */

    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if (semid == -1)
        errExit("semget");

    if (setEventFlag(semid, WRITE_SEM) == -1)
        errExit("setEventFlag");
    if (clearEventFlag(semid, READ_SEM) == -1)
        errExit("clearEventFlag");

    /* Create shared memory; attach at address chosen by system */

    shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
    if (shmid == -1)
        errExit("shmget");

    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *)-1)
        errExit("shmat");

    /* Transfer blocks of data from stdin to shared memory */

    for (xfrs = 0, bytes = 0;; xfrs++, bytes += shmp->cnt)
    {
        // 注意这种非原子性的操作容易出竞争bug，不过幸好这里是迭代而且只有这一处用到，所以没有竞争
        if (waitForEventFlag(semid, WRITE_SEM) == -1)
            errExit("waitForEventFlag");
        if (clearEventFlag(semid, WRITE_SEM) == -1)
            errExit("clearEventFlag");

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1)
            errExit("read");

        if (setEventFlag(semid, READ_SEM) == -1)
            errExit("setEventFlag");

        /* Have we reached EOF? We test this after giving the reader
           a turn so that it can see the 0 value in shmp->cnt. */

        if (shmp->cnt == 0)
            break;
    }

    /* Wait until reader has let us have one more turn. We then know
       reader has finished, and so we can delete the IPC objects. */

    if (waitForEventFlag(semid, WRITE_SEM) == -1)
        errExit("waitForEventFlag");
    if (clearEventFlag(semid, WRITE_SEM) == -1)
        errExit("clearEventFlag");

    if (semctl(semid, 0, IPC_RMID, dummy) == -1)
        errExit("semctl");
    if (shmdt(shmp) == -1)
        errExit("shmdt");
    if (shmctl(shmid, IPC_RMID, 0) == -1)
        errExit("shmctl");

    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
