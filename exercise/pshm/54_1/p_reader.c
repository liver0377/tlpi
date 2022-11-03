#include "pshm.h"

int
main(int argc, char *argv[])
{
    int semid, xfrs, bytes;
    int size = sizeof(struct shmseg);
    /* Get IDs for semaphore set and shared memory created by writer */

    semid = semget(SEM_KEY, 0, 0);
    if (semid == -1)
        errExit("semget");

    int sd;
    if ((sd = shm_open("/demo_shm", O_CREAT | O_RDONLY, 0644)) == -1)
           errExit("shm_open");

    struct shmseg *shmp;
    shmp = (struct shmseg*)  mmap(NULL, size, PROT_READ, MAP_SHARED,  sd, 0);
    if (shmp == MAP_FAILED)
            errExit("mmap");
   /* shmid  = shmget(SHM_KEY, 0, 0);
    if (shmid == -1)
        errExit("shmget");
   
    shmp = shmat(shmid, NULL, SHM_RDONLY);
    if (shmp == (void *) -1)
        errExit("shmat");
   */
    /* Transfer blocks of data from shared memory to stdout */

    for (xfrs = 0, bytes = 0; ; xfrs++) {
        if (reserveSem(semid, READ_SEM) == -1)          /* Wait for our turn */
            errExit("reserveSem");

        if (shmp->cnt == 0)                     /* Writer encountered EOF */
            break;
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
            fatal("partial/failed write");

        if (releaseSem(semid, WRITE_SEM) == -1)         /* Give writer a turn */
            errExit("releaseSem");
    }
    /*
    if (shmdt(shmp) == -1)
        errExit("shmdt");
    */
    /* Give writer one more turn, so it can clean up */
    if (munmap(shmp, size) == -1)
        errExit("munmap");

    if (releaseSem(semid, WRITE_SEM) == -1)
        errExit("releaseSem");

    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
