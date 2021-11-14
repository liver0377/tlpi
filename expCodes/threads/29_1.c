#include<pthread.h>
#include"../lib/tlpi_hdr.h"


void * threadFunc(void *arg){
	printf("threadFunc is called:\n");
	//pthread_ t t1;
	//if(pthread_equal(t1, pthread_seld()) != 0)	采用pthread_equal()判断一下即可
	if(pthread_join(pthread_self(), NULL) != 0)
		errExit("pthread_join");
	printf("thread exits\n");
	return (void *)0;
}

int main(int argc, char **argv){
	pthread_t tid;
	pthread_create(&tid, NULL, threadFunc, 0);
	sleep(3);			//让主线程后终止
	return 0;
}
