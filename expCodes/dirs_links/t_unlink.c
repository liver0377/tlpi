#include<sys/stat.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024

int main(int argc, char **argv){
	int fd, j, numBlocks;
	char shellCmd[CMD_SIZE];
	char buf[BUF_SIZE];

	if(argc < 2 || strcmp("--help", argv[1]) == 0)
		usageErr("$s tmp-file [num-1kB-blocks] \n", argv[0]);

	numBlocks = argc > 2 ? atoi(argv[2]) : 100000;

	fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if(fd == -1)
		errExit("open");

	if(unlink(argv[1]) == -1)
		errExit("unlink");

	for(j = 0; j < numBlocks; j++){
		if(write(fd, buf, BUF_SIZE) != BUF_SIZE)
			fatal("partial/failed write");
	}

	snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
	system(shellCmd);

	if(close(fd) == -1)
		errExit("close");

	printf("********************Closed file despritor\n");

	system(shellCmd);
	exit(EXIT_SUCCESS);
}
