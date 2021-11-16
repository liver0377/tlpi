#include<pthread.h>
#include<signal.h>
#include"../lib/tlpi_hdr.h"

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int glob;

static void 
cleanupHandler(void *arg){
	int s;
	printf("cleanup is called\n");

	printf("free the block\n");
	free(arg);

	printf("unlock the mutex\n");
	s = pthread_mutex_unlock(&mutex);

	printf("cleanup is over\n");
}

static void* 
threadFunc(void *arg){
	int s;
	void * buf = malloc(0x10000);
	if(buf == NULL)
		errExit("malloc");

	s = pthread_mutex_lock(&mutex);
	if(s != 0)
		errExit("pthread_mutex_lock");

	pthread_cleanup_push(cleanupHandler, buf);

	while(glob == 0){
		s = pthread_cond_wait(&cond, &mutex);
		if(s != 0)
			errExitEN(s, "pthread_cond_wait");
	}

	printf("condition loop completed\n");
	
	pthread_cleanup_pop(0);
	
	return NULL;
}	

int main(int argc, char **argv){
	pthread_t thr;
	void *res;
	int s;

	s = pthread_create(&thr, NULL, threadFunc, NULL);
	if(s != 0)
		errExitEN(s, "pthread_create");

	sleep(2);

	if(argc == 1){
		printf("about to cancle thread\n");
		s = pthread_cancel(thr);
		if(s != 0)
			errExitEN(s, "pthread_cancle");
	}else{
		glob = 1;
		printf("about to signal thread\n");
		s = pthread_cond_signal(&cond);
		if(s != 0)
			errExitEN(s, "pthrad_cond_signal");
	}

	s = pthread_join(thr, &res);
	if(s != 0)
		errExitEN(s,"pthread_join");

	if(res == PTHREAD_CANCELED)
		printf("thread was canceled\n");
	else
		printf("thread terminated normally\n");

	exit(EXIT_SUCCESS);
}


