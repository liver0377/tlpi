#include<sys/resource.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	if(argc < 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s nice-value program [args...]");

	int niceValue = atoi(argv[1]);
	
	//先改变当前进程的nice值，然后使用execvp覆盖掉当前进程，nice值保持不变
	if(setpriority(PRIO_PROCESS, 0, niceValue) == -1)
		errExit("setpriority");

	if(execvp(argv[2], argv + 2) == -1)
		errExit("execvp");

}
