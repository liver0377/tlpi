#include<pthread.h>
#include<stdio.h>
#include<signal.h>
#include"../lib/tlpi_hdr.h"
#include"../signals/signal_functions.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void*
threadFunc(void *msg){
	int s;
	if((s = pthread_mutex_lock(&mutex)) != 0)
		errExitEN(s, "pthread_mutex_lock");
	printf("threadFunc is called\n");
	printf("my signalpending is:");
	printPendingSigs(stdout, msg);
	if((s = pthread_mutex_unlock(&mutex)) != 0)
		errExitEN(s, "pthread_muetx_unlock");
}

static void*
specialThreadFunc(void* msg){
	pthread_t childPid;
	int s;
	
		
	switch(childPid = fork()){
		case -1:
			errExit("fork"); break;
		case 0:
			//直接结束
			_exit(EXIT_SUCCESS);
		default:
			//调用fork()的线程
			s = pthread_mutex_lock(&mutex);
			if(s != 0)
				errExitEN(s, "pthread_mutex_unlcok");
			printf("specialTrheadFunc is called\n");
			printf("my sigpending is :");
			printPendingSigs(stdout, msg);
			s = pthread_mutex_unlock(&mutex);
			if(s != 0)
				errExitEN(s, "pthread_mutex_unlock");
			break;
	}
}

int main(int argc, char **argv){
	pthread_t t1, t2;
	int s;
	char* msg;

	sigset_t set;
	if(sigaddset(&set, SIGUSR1) == -1)
		errExit("sigaddset");
	if(sigaddset(&set, SIGUSR2) == -1)
		errExit("sigaddset");
	if(sigaddset(&set, SIGCHLD) == -1)
		errExit("sigaddset");
	if(sigprocmask(SIG_BLOCK, &set, NULL) == -1)
		errExit("sigprocmask");  	//在首线程中阻塞SIGUSR1和SIGUSR2信号，创建的线程会继承进程的信号掩码

	msg = "(thread 1)";
	s = pthread_create(&t1, NULL, threadFunc, msg);
	if(s != 0)
		errExitEN(s, "pthread_create");

	msg = "(thread 2)";
	s = pthread_create(&t2, NULL, specialThreadFunc, msg);
	if(s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_kill(t1,SIGUSR1);
	if(s != 0)
		errExitEN(s, "pthread_kill");

	s = pthread_kill(t2, SIGUSR2);
	if(s != 0)
		errExitEN(s, "pthread_kill");

	if(pthread_join(t1, NULL) != 0)
		errExitEN(s, "thread_join");
	if(pthread_join(t2, NULL) != 0)
		errExitEN(s, "thread_join");

	return 0;
}

