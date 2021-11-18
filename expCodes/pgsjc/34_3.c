#define _XOPEN_SOURCE 500
#include<unistd.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	pid_t childPid;

	switch(childPid = fork()){
		case -1:
			errExit("fork");
		case 0:
			if(setpgid(0, 0) == -1)
				errExit("setpgid");
			if(setsid() == (pid_t) -1)
				errExit("setgid");
			break;
		default:
			sleep(1);
			break;
		}
	return 0;
}
