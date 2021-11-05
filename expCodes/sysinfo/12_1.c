#include<stdio.h>
#include<sys/stat.h>
#include"../lib/tlpi_hdr.h"
#include"../users_groups/ugid_functions.h"
#include<dirent.h>
#include<fcntl.h>
#include<assert.h>
#include<pwd.h>
#include<ctype.h>

#define BUF_SIZE 1024

const char* parsing_name1(const char* s){
	static char path[_POSIX_PATH_MAX + 10];
	sprintf(path, "/proc/%s/status", s);
	return path;
}

const char* parsing_name2(const char* s){
	static char path[_POSIX_PATH_MAX];
	sprintf(path, "/proc/%s/cmdline", s);
	return path;
}

Boolean isNumericalStr(const char* s){
	int len = strlen(s);
	for(int i = 0; i < len; i++){
		if(!(s[i] >='0' && s[i] <= '9')) return 0;
	}
	return 1;
}



int main(int argc, char **argv){
	DIR* dir;
	struct dirent* dr;
	FILE* f1;
	FILE* f2;
	char buf[BUF_SIZE], staBuf[BUF_SIZE], cmdBuf[BUF_SIZE];  
	uid_t uid, curUid;
	pid_t pid;
	
	if(argc != 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s username ", argv[0]);
	
	printf("User: %s\n", argv[1]);	
	
	//获取输入用户的uid
	uid = userIdFromName(argv[1]);

	dir = opendir("/proc");
	if(dir == NULL)
		errExit("opendir");

	
	while((dr = readdir(dir)) != NULL){
		if(!isNumericalStr(dr->d_name)) continue;
		f1 = fopen(parsing_name1(dr->d_name), "r");
		if(f1 == NULL)
			errExit("open");
		while(fgets(buf, BUF_SIZE, f1)){
		//截取当前正在运行的程序
			if(strncmp(buf, "State:", 6) == 0)
				sscanf(buf + 6, " %s ", staBuf);
		
			if(strncmp(buf, "Pid:", 4) == 0)
				sscanf(buf + 4, " %d ", &pid);

			if(strncmp(buf, "Uid:", 4) == 0)
				sscanf(buf + 4, " %d", &curUid);

		}	

		if(fclose(f1) == EOF)
			errExit("close");

		f2 = fopen(parsing_name2(dr->d_name), "r");
		if(f2 == NULL)
			errExit("open");

		fgets(cmdBuf, BUF_SIZE, f2);

		if(fclose(f2) == EOF)
			errExit("close");

		if(uid == curUid ){
		    printf("State:  %s  ", staBuf);
		    printf("Pid:    %d  ", pid);
		    printf("cmdline:%s\n", cmdBuf);
		}
	}
	return 0;
}

