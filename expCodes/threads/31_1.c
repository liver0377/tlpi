#define _GNU_SOURCE 
#include<pthread.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

struct ctol{
	Boolean isUsed;
	pthread_mutex_t mutex;	//为了控制对变量isUsed的访问
}

int one_time_init(struct ctol *control, void (*init)(void)){
	int s;

	s = pthread_mutex_lock(control->mutex);
	if(s != 0)
		errExitEN(s, "pthread_mutex_lock");
	if(control->isUsed == 0){
		isUsed = 1;
		init();
	}
	return 0;
}

