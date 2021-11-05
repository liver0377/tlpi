#define _GUN_SOURCE
#include<stdlib.h>
#include"../lib/tlpi_hdr.h"

extern char** environ;

int main(int argc,char** argv){
	int j;
	char** ep;

	clearenv();					//清除整个环境
	for(j = 1; j < argc; j++){
		if(putenv(argv[j]) != 0)		//正确时返回0
				errExit("putenv: %s", argv[j]);
	}

	if(setenv("GREET", "HELLO WORLD", 0) == -1)
		errExit("setenv");

	unsetenv("BYE");

	for(ep = environ; *ep!=NULL; ep++)
		puts(*ep);
	
	exit(EXIT_SUCCESS);
	}
