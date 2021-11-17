#include<unistd.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int s;
	s = sysconf(_SC_THREAD_STACK_MIN);
	if(s == -1)
		errExit("sysconf");
	printf("%d\n", s);

	return 0;
}
