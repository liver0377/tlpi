#include<sched.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	if(argc < 4 || strcmp(argv[1], "--help") == 0)
		usageErr("%s {r|f} priority command [arg...]"
		         "r is SCHED_RR, f is SCHED_FIFO", argv[0]);
	int policy;
	struct sched_param param;
	
	policy = argv[1][0] == 'r' ? SCHED_RR :
		 argv[1][0] == 'f' ? SCHED_FIFO :
		 SCHED_OTHER;

	param.sched_priority = atoi(argv[2]);

	if(sched_setscheduler(0, policy, &param) == -1)
		errExit("sched_setscheduler");
	//将自身的real, effective, saved uid均设为原ruid
	if(setuid(getuid()) == -1)
		errExit("setuid");

	if(execvp(argv[3], argv + 4) == -1)
		errExit("execvp");

}

