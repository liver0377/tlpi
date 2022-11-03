#include<sys/stat.h>
#include"../lib/tlpi_hdr.h"

#define BUF_SIZE  1024
int main(int argc, char **argv){
	if(argc < 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s file", argv[0]);
		
	//判断文件的类型
	struct stat statBuf;	
	Boolean b;              //0 is dir, 1 is file

	for(int i = 0; i < argc - 1; i++){

	if(stat(argv[i + 1], &statBuf) == -1)
		 errExit("stat");

	b = S_ISDIR(statBuf.st_mode) ? 0 : 1;
	mode_t mode = statBuf.st_mode | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

	//1.目录
	if(!b){
		if(chmod(argv[i + 1], mode) == -1)
			errExit("chmod");
	} else{
	//2.文件
		if((statBuf.st_mode & S_IXUSR) || (statBuf.st_mode & S_IXGRP) || (statBuf.st_mode & S_IXOTH)){
     	 		if(chmod(argv[i + 1], mode) == -1)
				errExit("chmod");
		}else{
			mode = statBuf.st_mode | S_IRUSR | S_IRGRP | S_IROTH;
			if(chmod(argv[i + 1], mode) == -1)
				errExit("chmod");
		}
	}
	}
	return 0;
}
		
