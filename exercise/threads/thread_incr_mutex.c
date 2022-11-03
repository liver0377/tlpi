#include<pthread.h>
#include"../lib/tlpi_hdr.h"

static int glob = 0;
static pthread_mutex_t mtk = PTHREAD_MUTEX_INITIALIZER;
static void* threadFunc(void *arg){
	int loops = *(int *)arg;
	int loc;

	for(int i = 0; i < loops; i++){
		int s;
		s = pthread_mutex_lock(&mtk);
		if(s != 0) 
		    errExitEN(s, "pthread_mutex_lock");	
		loc = glob;
		loc++;
		glob++;

		s = pthread_mutex_unlock(&mtk);
		if(s != 0)
		    errExitEN(s, "pthread_mutex_unlock");
	}
}

int main(int argc, char **argv){
	int s;
	int loops;
	pthread_t t1, t2;
	loops = (argc > 1) ? atoi(argv[1]) : 1000000;

	s = pthread_create(&t1, NULL, threadFunc, &loops);
	if(s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_create(&t2, NULL, threadFunc, &loops);
	if(s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_join(t1, NULL);
	if(s != 0)
		errExitEN(s, "pthread_join");

	s = pthread_join(t2, NULL);
	if(s != 0)
		errExitEN(s, "pthread_join");

	printf("glob is %d now \n", glob);

	return 0;
}
