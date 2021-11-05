#ifndef FILE_PERMS_H
#define FILE_PERMS_H

#include<sys/stat.h>

#define FP_SPECIAL 1  		//是否显式文件的suid, sgid, sbit位

char *filePermStr(mode_t perm, int flags);

#endif
