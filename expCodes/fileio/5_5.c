#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char** argv){
	if(argc != 2 || strcmp(argv[1], "--help")==0)
		usageErr("%s file", argv[0]);
	int fd; int newfd;
	int origin; int modified; int Dmodified;  //Dmodified为副本修改后 
	int flags;
	fd = open(argv[1], O_CREAT | O_RDONLY , S_IRUSR | S_IWUSR);

	if(fd == -1) 
	 	errExit("open");
	
	newfd = dup(fd);
	if(newfd == -1)
		errExit("dup");

	//通过newfd进行文件偏移量的修改
	if((origin = lseek(fd, 0, SEEK_CUR)) == -1)
		errExit("lseek");
	if((Dmodified = lseek(newfd, 1, SEEK_CUR)) == -1)
		errExit("lseek");
	if((modified = lseek(fd, 0, SEEK_CUR)) == -1)
		errExit("lseek");
	printf("fd origin: %d\n",origin);
	printf("fd modified: %d\n",modified);
	printf("newfd modified: %d\n",Dmodified);

	//通过newfd进行文件状态的修改
	
	printf("%s don't have flag: O_APPEND\n", argv[1]);
	flags = fcntl(fd, F_GETFL);
	if(flags == -1)
		errExit("fcntl");
	flags |= O_APPEND;
	if(fcntl(newfd, F_SETFL, flags)==-1)
		errExit("fcntl");
	flags = fcntl(fd, F_GETFL);
	if(flags == -1)
		errExit("fcntl");
	if(flags & O_APPEND)
		printf("%s now have flag: O_APPEND\n", argv[1]);
	else
		printf("%s now don't have flag: O_APPEND\n", argv[1]);
	exit(EXIT_SUCCESS);
}

	

