#define _POPSIX_C_SOURCE 199309
#include<signal.h>
#include<time.h>
#include<sys/time.h>
#include"../time/curr_time.h"
#include"itimerspec_from_str.h"
#include"../lib/tlpi_hdr.h"

#define TIMER_SIG SIGRTMAX


int main(int argc, char *argv[]){
	struct itimerspec ts;
	struct sigevent sev;
	timer_t *tidlist;
	int j;

	if(argc < 2) 
		usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...n", argv[0]);

	tidlist = calloc(argc - 1, sizeof(timer_t));
	if(tidlist == NULL)  errExit("malloc");


	sev.sigev_notify = SIGEV_SIGNAL;			//表明信号到期时将会发送信号，信号值为sa.sigev_signo
	sev.sigev_signo = TIMER_SIG;

	for(j = 0; j < argc - 1; j++){
		itimerspecFromStr(argv[j + 1], &ts);

		sev.sigev_value.sival_ptr = &tidlist[j];

		if(timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1)  errExit("timer_create");

		printf("Timer ID: %ld (%s)\n", (long) tidlist[j], argv[j + 1]);

		if(timer_settime(tidlist[j], 0, &ts, NULL) == -1)  errExit("timer_settime");
	}
	sigset_t set;
	if(sigemptyset(&set) == -1) errExit("sigemptyset");
	if(sigaddset(&set, TIMER_SIG) == -1) errExit("sigaddset");

	siginfo_t info;
	for(;;){
	    sigwaitinfo(&set, &info);
	    timer_t *tidptr;
	    tidptr = info.si_value.sival_ptr;

	    printf("[%s] Got signal %d\n", currTime("%T"), info.si_signo);
	    printf("	*sival_ptr   	    = %ld\n", (long) *tidptr);
	} 
}	  



