#include<signal.h>
#include"../lib/tlpi_hdr.h"
#include<stdio.h>


static void signalHandler(int sig){
	printf("Ouch\n");
}

int main(int argc, char **argv){
	if(signal(SIGINT, signalHandler) == SIG_ERR)
		errExit("signal");

	for(int j = 0; ; j++){
		printf("%d\n", j);
		sleep(3);
	}
}
