#include<unistd.h>
#include<sys/stat.h>
#include"../lib/tlpi_hdr.h"


#define BUF_SIZE 1024

int main(int argc, char **argv){
	//检查参数的正确性，(是否为链接?）,(参数的数目?)
	if(argc != 2 || strcmp("--help", argv[1]) == 0)
		usageErr("%s linkfile", argv[0]);

	struct stat statBuf;
	if(lstat(argv[1], &statBuf) == -1)
		errExit("lstat");

	if(!S_ISLNK(statBuf.st_mode))
		fatal("%s is not a symbolic link", argv[1]);

	char buf[BUF_SIZE];
	int numbytesRead;
	if((numbytesRead = readlink(argv[1], buf, BUF_SIZE)) == -1)
		errExit("readlink");
	//由于readlink不会在buf末尾添加上\0，所以需要手动添加
	buf[numbytesRead] = '\0';

	printf("%s ===>  %s\n", argv[1], buf);

	exit(EXIT_SUCCESS);
}
