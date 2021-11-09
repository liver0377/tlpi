#include<time.h>
#include<sys/time.h>
#include"../lib/tlpi_hdr.h"

unsigned int
alarm(unsigned int seconds){
	struct itimerval val;

	val.it_interval.tv_sec = 0;
	val.it_interval.tv_sec = 0;
	val.it_value.tv_sec = seconds;
	val.it_value.tv_usec = 0;

	if(setitimer(ITIMER_REAL, &val, NULL) == -1) errExit("settimer");

	unsigned int remain;
	struct itimerval remain_value;
	if(getitimer(ITIMER_REAL, &remain_value) == -1) errExit("gettimer");

	remain = remain_value.it_value.tv_sec;
	
	return remain;
}

int main(int argc, char **argv){
	return 0;
}
