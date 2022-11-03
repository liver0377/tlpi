#define _XOPEN_SOURCE 500
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/wait.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	if(execvp("/home/liver0377/tlpi/expCodes/pgsjc/newprog.out", NULL) == -1)
		errExit("execnp");
}
