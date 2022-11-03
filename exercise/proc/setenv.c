#include<stdlib.h>
#include"../lib/tlpi_hdr.h"
#include<string.h>

#define BUF_SIZE 1024
char buf[BUF_SIZE];

extern char** environ;
int main(int argc, char** argv){
	int len1,len2;
	if(argc != 4 || strcmp(argv[1] , "--help")==0)
		usageErr("%s name value\n", argv[0]);

	for(char** ep = environ; *ep!=NULL; ep++)
		puts(*ep);

	//将name和value写入缓存
	len1 = strlen(argv[1]);
	len2 = strlen(argv[2]);
	strcpy(buf,argv[1]);
	buf[len1]='=';
	strcpy(buf+len1+1,argv[2]);

	//使用putenv
	if(atoi(argv[3]) == 1){
	    if(putenv(buf) != 0)
		errExit("putenv");
	}
	else{
		if(getenv(argv[1]) ==NULL)
		   if(putenv(buf) != 0)
		      errExit("putenv");


	}	
	//打印环境列表
	for(char** ep = environ; *ep!=NULL; ep++)
		puts(*ep);
	exit(EXIT_SUCCESS);
}
