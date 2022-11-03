#define _GNU_SOURCE
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include"../lib/tlpi_hdr.h"


static void* 
threadFunc(void *arg){
	printf("threadFunc is called now:\n");
	
	for(int i = 1; ; i++){
		printf("loop: %d\n", i);
	}

	return NULL;
}

int main(int argc, char **argv){
	pthread_t thr;
	int s;
	void *retval;

	s = pthread_create(&thr, NULL, threadFunc, NULL);
	if(s != 0)
		errExitEN(s, "pthread_create");

	sleep(3);			//让线程运行一会

	s = pthread_cancel(thr);
	if(s != 0)
		errExitEN(s, "pthread_cancle");

	printf("pthrad_join is called\n");
	s = pthread_join(thr, &retval);
	if(s != 0) 
		errExitEN(s, "pthread_join");

	if(retval == PTHREAD_CANCELED)
		printf("thread was cancled\n");
	else
		printf("thread was not cancled\n");

	return 0;
}
