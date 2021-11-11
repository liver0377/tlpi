#include"../lib/tlpi_hdr.h"

static int idata = 111;

int
main(int argc, char *argv[]){
	int istack = 222;
	pid_t childPid;

	switch(childPid = fork()){
		case -1:
			errExit("fork");    break;
		case 0:
			idata *= 3;
			istack *= 3;
			break;
		default:
			sleep(3);		//目的是让子进程能够先执行完，但不一定万不一失
			break;
	}
	
	printf("PID=%ld %s idata=%d istack=%d\n", (long) getpid(),
			(childPid == 0) ? "(child)" :"(parent)", idata, istack);

	exit(EXIT_SUCCESS);
}
