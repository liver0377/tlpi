#define _POSOX_C_SOURCE 199309
#include<sys/time.h>
#include<time.h>
#include<signal.h>
#include"../lib/tlpi_hdr.h"


static void 
sigintHandler(int sig){
	return;			//简单返回
}

int main(int argc, char **argv){
	struct timeval start, finish;
	struct timespec request, remain;
	struct sigaction sa;

	if(argc != 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s secs nanosecs\n", argv[0]);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sigintHandler;
	if(sigaction(SIGINT, &sa, NULL) == -1) errExit("sigaction");

	if(gettimeofday(&start, NULL) == -1) errExit("gettimeofday");

	struct timespec tp;
	if(clock_gettime(CLOCK_REALTIME, &tp) == -1) errExit("clock_gettime");		//获取到当前时钟时间

	request.tv_sec = getLong(argv[1], 0, "secs") + tp.tv_sec;			//在此基础上加上给出的休眠时间
	request.tv_nsec = getLong(argv[1], 0, "nanosecs") + tp.tv_nsec;

	for(;;){
		int s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, &remain);
		if(s == -1 && errno != EINTR) errExit("nanosleep");

		if(gettimeofday(&finish, NULL) == -1) errExit("gettimeofday");
		
		printf("Slept for %9.6f secs\n", finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec) / 1000000.0);

		if(s == 0) break;		//执行完毕

		printf("Remaing: %2ld.%09ld\n", (long) remain.tv_sec, remain.tv_nsec);

	}

	printf("Sleep complete\n");
	exit(EXIT_SUCCESS);
}

	

