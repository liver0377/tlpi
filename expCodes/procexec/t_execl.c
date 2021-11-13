#include<stdlib.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	printf("Initial value of USER: %s\n", getenv("USER"));
	if(putenv("USER=britta") != 0)
		errExit("putenv");

	execl("/usr/bin/printenv", "prinenv", "USER","SHELL", (char *) NULL);
	errExit("execl");
}
