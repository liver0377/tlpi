#define _XOPEN_SOURCE 500
#include<sys/stat.h>
#include<ftw.h>
#include<stdio.h>
#include"../lib/tlpi_hdr.h"

int func(const char *pathname, const struct stat *statbuf, int typeflag, struct FTW *ftwbuf){
	printf("pathnanme:  %s\n", pathname);
	return 0;
}
int main(int argc, char **argv){
	if(nftw(argv[1], func, 1000, 0) == -1)
		errExit("nftw");
	return 0;
}

