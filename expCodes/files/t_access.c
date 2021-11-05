#include<unistd.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"


int main(int argc, char **argv){
	if(argc != 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s file", argv[0]);

	char str[4] = {'-', '-', '-', '-'};
	
	if(access(argv[1], F_OK) == 0)
		str[0] = 'e';
	if(access(argv[1], R_OK) == 0)
		str[1] = 'r';
	if(access(argv[1], W_OK) == 0)
		str[2] = 'w';
	if(access(argv[1], X_OK) == 0)
		str[3] = 'x';
	for(int i = 0; i < 4; i++)
		printf("%c", str[i]);
	printf("\n");
	return 0;
}
