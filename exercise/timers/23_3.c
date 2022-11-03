#include<sys/time.h>
#include<time.h>
#include<signal.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

void handler(int sig, siginfo_t *info, void *ut){
       if(sig == SIGALRM)
	  printf("Caught SIGALARM\n");
       printf("info->si_value.sival_int: %d\n", info->si_value.sival_int);
}

int main(int argc, char **argv){
	timer_t timerid;
	struct sigaction sa;

	if(sigemptyset(&sa.sa_mask) == -1) errExit("sigemptyset");
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	
	//为SIGALARM信号设置处理器函数
	if(sigaction(SIGALRM, &sa, NULL) == -1) errExit("saction");
	if(timer_create(CLOCK_REALTIME, NULL, &timerid) == -1) errExit("time_create");

	struct itimerspec curr_value;
	curr_value.it_interval.tv_sec = 1;
	curr_value.it_interval.tv_nsec =0;
	curr_value.it_value.tv_sec = 1;
	curr_value.it_value.tv_nsec = 0;

	if(timer_settime(timerid, 0, &curr_value, NULL) == -1) errExit("timer_gettime");

	for(;;)
		pause();
}

