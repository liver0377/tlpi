#define _POSIX_C_SOURCE 199309
#include<signal.h>
#include<string.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int sig, numSigs, j, sigData;
	union sigval sv;

	if(argc < 4 || strcmp(argv[1], "--help")  == 0)
		usageErr("%s pid sig-num data [num-sigs]\n", argv[0]);

	printf("%s: PID is %ld, UID is %ld\n", argv[0], (long)getpid(), (long)getuid());

	sig = atoi(argv[2]);
	sigData = atoi(argv[3]);
	numSigs = (argc > 4) ? atoi(argv[4]) : 1;

	for(j = 0; j < numSigs; j++){
		sv.sival_int = sigData + j;
		if(sigqueue(getLong(argv[1], 0, "pid"), sig, sv) == -1)
			errExit("sigqueue %d", j);
	}

	exit(EXIT_SUCCESS);
}
