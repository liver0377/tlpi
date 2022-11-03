#include<signal.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int s, sig;

	if(argc != 3 || strcmp("--help", argv[1]) == 0)
		usageErr("%s sig-num pid\n", argv[0]);

	sig = atoi(argv[1]);

	s = kill(atoi(argv[2]), sig);

	if(sig != 0){
		if(s == -1)
			errExit("kill");
	}else{
		if(s == 0){
			printf("the process exists\n");
		}
		else if(errno == EPERM)
			printf("process exists but we don't have the permission to access it\n");
		else if(errno == ESRCH)	
			printf("process does not exist\n");
		else
			errExit("kill");
	}

	errExit(EXIT_SUCCESS);
}
	
