#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include"../lib/tlpi_hdr.h"

static int glob = 0;
static sem_t sem;

static void *
threadFunc(void *arg){
	int loops = *((int *) arg);
	int loc, j;

	for (j = 0; j < loops; j++){
		if (sem_wait(&sem) == -1)
			errExit("sem_wait");
		loc = glob;
		loc++;
		glob = loc;

		if (sem_post(&sem) == -1)
			errExit("sem_post");

	}
	return NULL;
}

int main(int argc, char **argv){
	pthread_t t1, t2;
	int loops, s;

	loops = (argc > 1) ? atoi(argv[1]) : 1000000;

	if (sem_init(&sem, 0, 1) == -1)
		errExit("sem_init");

	s = pthread_create(&t1, NULL, threadFunc, &loops);
	if (s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_create(&t2, NULL, threadFunc, &loops);
	if (s != 0)
		errExitEN(s, "pthred_create");

	s = pthread_join(t1, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");

	s = pthread_join(t2, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");

	printf("glob = %ld\n", (long) glob);
	exit(EXIT_SUCCESS);
}
