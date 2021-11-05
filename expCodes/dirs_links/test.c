#define _XOPEN_SOURCE 500
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include"../lib/tlpi_hdr.h"

#define BUF_SIZE 1024

char *getcwd(char *buf, size_t sz){
	int initialFd;
	long finode;
	long sinode;
	struct stat statBuf;
	int p = sz;						//指向最终路径名首部在buf中的位置

	initialFd = open(".", O_RDONLY);    		//记录下初始的文件描述符，后来使用fchdir返回到初始工作目录
	if(initialFd == -1)
		errExit("open");

	for(;;){
		//开始扫描当前目录
		struct dirent *item;
		DIR *dirp;
		int len;

		if(stat(".", &statBuf) == -1) errExit("stat");
		sinode = statBuf.st_ino;

		if(stat("..", &statBuf) == -1) errExit("stat");
		finode = statBuf.st_ino;			//获取当前目录和父亲目录的inode编号

		printf("sinode: %ld   finode: %ld \n", sinode, finode);
		if(finode == sinode) break;			//表明已经遍历到目录的顶点

		if(chdir("..") == -1) errExit("chdir");		//获取完inode之后切换到父目录
		//if(opendir(".") == NULL) printf("sorry, it's empty");
		while((dirp = opendir(".")) != NULL){
			printf("I am In !\n");
			if((item = readdir(dirp)) == NULL) errExit("readdir"); 	
			
			printf("item->d_name： %s\n", item->d_name);
			if(strcmp(".", item->d_name) == 0 || strcmp("..", item->d_name) == 0)  continue;

			sinode = item->d_ino;
			printf("current sinode is : %ld\n", sinode);
			
			if(finode == sinode){			//如果从当前目录的子目录中找到inode号与当前目录的inode
				int t = p - 1;
				printf("item->d_name: %s\n", item->d_name);
				p = p -strlen(item->d_name) -1;
				strcpy(&buf[p], item->d_name);
				buf[t] = '/';
			}

		
		}
	}


	buf[sz - 1] ='\0';
	buf[p - 1] = '/';
	return &buf[p - 1];
}


int main(int argc, char **argv){
	char *p;
	char buf[BUF_SIZE];
	p = getcwd(buf, BUF_SIZE);
	printf("current path: %s\n", p);
	return 0;
}




