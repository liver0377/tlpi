#include "pshm.h"

int
main(int argc, char *argv[])
{
    int semid, bytes, xfrs;
    union semun dummy;
    int size = sizeof(struct shmseg);
    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if (semid == -1)
        errExit("semget");
  
    if (initSemAvailable(semid, WRITE_SEM) == -1)
        errExit("initSemAvailable");
    if (initSemInUse(semid, READ_SEM) == -1)
        errExit("initSemInUse");

    int sd;
    sd = shm_open("/demo_shm", O_RDWR, 0666);
    if (sd == -1)
            errExit("shm_open");
    struct shmseg *shmp;
    shmp =(struct shmseg*)  mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, sd, 0);
    if (shmp == MAP_FAILED)
           errExit("mmap");
    /*shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
    if (shmid == -1)
        errExit("shmget");

    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1)
        errExit("shmat");
    */


    for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
        if (reserveSem(semid, WRITE_SEM) == -1)         /* Wait for our turn */
            errExit("reserveSem");

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1)
            errExit("read");

        if (releaseSem(semid, READ_SEM) == -1)          /* Give reader a turn */
            errExit("releaseSem");

        /* Have we reached EOF? We test this after giving the reader
           a turn so that it can see the 0 value in shmp->cnt. */

        if (shmp->cnt == 0)
            break;
    }

    /* Wait until reader has let us have one more turn. We then know
       reader has finished, and so we can delete the IPC objects. */

    if (reserveSem(semid, WRITE_SEM) == -1)
        errExit("reserveSem");

    if (semctl(semid, 0, IPC_RMID, dummy) == -1)
        errExit("semctl");
    if (munmap(shmp, size) == -1)
    /*
    if (shmdt(shmp) == -1)
        errExit("shmdt");
    
    if (shmctl(shmid, IPC_RMID, 0) == -1)
        errExit("shmctl");
    */
    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
