#include<stdio.h>
#include<stdarg.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	printf("hello world\n");
	execlp("/bin/sleep", "sleep", 0, (char *) NULL);
}
