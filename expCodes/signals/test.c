#define  _DEFAULT_SOURCE 
#include<stdio.h>
#include<signal.h>
#include"../lib/tlpi_hdr.h"
#include"signal_functions.h"


extern const char * const sys_siglist[];

int main(int argc, char **argv){
	FILE* f = fopen(argv[1], "w");
	printSigMask(f, "SigMask:");

	printPendingSigs(f, "PendingSigs:");
	return 0;	
}


