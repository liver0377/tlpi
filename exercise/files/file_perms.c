#include<sys/stat.h>
#include<stdio.h>
#include"file_perms.h"

#define STR_SIZE sizeof("rwxrwxrwx")

//将文件权限位掩码转化为字符串
char* filePermStr(mode_t perm, int flag){
	static char str[STR_SIZE];
	
	if(perm & S_IRUSR) str[0] = 'r';
	else		   str[0] = '-';
	
	if(perm & S_IWUSR) str[1] = 'w';
	else		   str[1] = '-';
	
	if(perm & S_IXUSR) {
	       	if((perm & S_ISUID) && (flag & FP_SPECIAL)) 
			   str[2] = 's';
		else                                        
                           str[2] = 'x';
	} else{
	       	if((perm & S_ISUID) && (flag & FP_SPECIAL)) 
		           str[2] = 'S';
		else
	     		   str[2] = '-';
	}

	if(perm & S_IRGRP) str[3] = 'r';
	else               str[3] = '-';

	if(perm & S_IWGRP) str[4] = 'w';
	else               str[4] = '-';

	if(perm & S_IXGRP) {
		 if((perm & S_ISGID) && (flag & FP_SPECIAL)) 
			   str[5] = 's';
		else                                        
                           str[5] = 'x';
	} else{
	       	if((perm & S_ISGID) && (flag & FP_SPECIAL)) 
		           str[5] = 'S';
		else
	     		   str[5] = '-';
	}

	if(perm & S_IROTH) str[6] = 'r';
	else               str[6] = '-';

	if(perm & S_IWOTH) str[7] = 'w';
	else               str[7] = '-';

	if(perm & S_IXOTH) {
		 if((perm & S_ISVTX) && (flag & FP_SPECIAL)) 
			   str[8] = 't';
		else                                        
                           str[8] = 'x';
	} else{
	       	if((perm & S_ISVTX) && (flag & FP_SPECIAL)) 
		           str[8] = 'T';
		else
	     		   str[8] = '-';
	}		
	     
	return str;
}
