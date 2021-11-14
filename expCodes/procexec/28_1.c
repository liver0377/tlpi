#include<sys/time.h>
#include<sys/wait.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	struct timeval time1, time2;
	pid_t childPid;
	int count;

	if(argc > 1 && strcmp(argv[1], "--help") == 0)
		usageErr("%s number", argv[0]);

	count = (argc > 1) ? atoi(argv[1]) : 10000000;
	if(gettimeofday(&time1, NULL) == -1) errExit("gettimeofday1");

	for(int i = 0; i < count; i++){
		switch(childPid = fork()){
			case -1:
				errExit("fork");
			case 0:
				_exit(EXIT_SUCCESS);
			default:
				if(waitpid(-1, NULL, 0) == -1)  errExit("waitpid");
				break;
		}	
	}
	if(gettimeofday(&time2, NULL) == -1) errExit("gettimeofday2");
	printf("fork() total time : %ld\n", time2.tv_sec - time1.tv_sec);

	if(gettimeofday(&time1, NULL) == -1) errExit("gettimeofday1");
	for(int i = 0; i < count; i++){
		switch(childPid = vfork()){
			case -1:
				errExit("fork");
			case 0:
				_exit(EXIT_SUCCESS);
			default:
				if(waitpid(-1, NULL, 0) == -1) errExit("waitpid");
				break;
	}
	}
	if(gettimeofday(&time2, NULL) == -1) errExit("getitmeofday2");
	printf("vfork() total time: %ld\n", time2.tv_sec - time1.tv_sec);
	return 0;
}
