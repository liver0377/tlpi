#include<sys/stat.h>
#include<ctype.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"

#define BUF_SIZE 1024
char buf[BUF_SIZE];

int main(int argc,char** argv){
	int fd;
	int numRead;
	int numWrite;
	if(argc != 2 || strcmp(argv[1] , "--help")==0)
		usageErr("%s pathname  offset\n");
	fd = open(argv[1], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	if(fd == -1)
		errExit("open");

	if(lseek(fd, 0, SEEK_SET) == -1)
		errExit("lseek");

	numRead= read(STDIN_FILENO, buf, BUF_SIZE);
	if(numRead == -1)
		errExit("read");
	numWrite = write(fd, buf, numRead);
	if(numWrite == -1)
		errExit("write");
	exit(EXIT_SUCCESS);
}
	
	
