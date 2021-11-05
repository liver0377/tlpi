#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include"../lib/tlpi_hdr.h"


#define BUF_SIZE 1024
//extern optind,opterr,optopt;
//extern char *optarg;

char buf[BUF_SIZE];

int 
main(int argc ,char** argv){
	char opt='\0';
	while((opt=getopt(argc,argv,"a:"))!=-1){
		//有选项a的情况
		//选项为a，在已存在的文件后追加内容
		        if(argc <3){
				usageErr("%s  [-a file] | [file]",argv[0]);
			int fd = open(argv[2], O_WRONLY | O_APPEND);   //只写，并且采用追加形式
		        if(fd == -1)
				errExit("open");
			while(1){
				int numRead = read(STDIN_FILENO, buf, BUF_SIZE);
				if(numRead == -1)
					errExit("read");		//读取失败
				int numWritten = write(STDOUT_FILENO, buf,numRead);
			        if(numWritten  == -1)
			       		errExit("wirte");		//写入失败
		 		numWritten = write(fd,buf,numRead);
				if(numWritten == -1)
					errExit("write");		//写入失败
			}
			free(buf);
			return 0;
		}
	}
	if(argc == 2){
		int perms = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH;
		int flags = O_CREAT | O_WRONLY | O_TRUNC;
		int fd = open(argv[1],flags,perms);
		if(fd == -1)
			errExit("open");
		 while(1){
                                int numRead = read(STDIN_FILENO, buf, BUF_SIZE);
                                if(numRead == -1)
                                        errExit("read");                //读取失败
                                int numWritten = write(STDOUT_FILENO, buf,numRead);
                                if(numWritten  == -1)
                                        errExit("wirte");               //写入失败
                                numWritten = write(fd,buf,numRead);
                                if(numWritten == -1)
                                        errExit("write");               //写入失败
                        }
                        free(buf);
                        return 0;
	}
	
	return 0;
}
