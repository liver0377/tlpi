#define _POSIX_C_SOURCE 199309
#include<string.h>
#include<stdlib.h>
#include"itimerspec_from_str.h"

//将时间字符串(/ :)转化为itimersepc值
void itimerspecFromStr(char *str, struct itimerspec *tsp){
	char *cptr, *sptr;

	cptr = strchr(str, ':');	//返回':'在字符串str中出现的第一个位置
	if(cptr != NULL)
		*cptr = '\0';

	sptr = strchr(str, '/');

	if(sptr != NULL)
		*sptr = '\0';

	//设置定时器计时时间
	tsp->it_value.tv_sec = atoi(str);
	tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

	//设置间隔时间
	if(cptr == NULL){
		//默认情况下是一次性计时器
		tsp->it_interval.tv_sec = 0;
		tsp->it_interval.tv_nsec = 0;
	}else{
		//寻找计时器间隔时间
		sptr = strchr(cptr + 1, '/');
		if(sptr != NULL)
			*sptr = '\0';
		tsp->it_interval.tv_sec = atoi(cptr + 1);
		tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
	}
}


