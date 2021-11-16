#define _GNU_SOURCE
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include"../lib/tlpi_hdr.h"
#define MAX_ERROR_LEN 256

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;			//全局变量

static void 
destructor(void *buf){
	printf("destructor is called\n");
	free(buf);
}

static void
createKey(void){
	int s;

	s = pthread_key_create(&strerrorKey, destructor);	//创建一个key,这个key虽然是全局变量，
							//但是通过setspecific可以使用相同的key来保存各个线程自己私有的数据
	if(s != 0)
		errExitEN(s, "pthread_create");

}

char *
strerror(int err){
	int s;
	char *buf;

	s = pthread_once(&once, createKey);		//当线程第一次调用strerror时会使用creatKey函数进行初始化，并且会修改once变量，但这个变量在进行修改后
							//就不会改变了
	if(s != 0)
		errExitEN(s, "pthread_once");

	buf = pthread_getspecific(strerrorKey);		//当线程刚刚创建的时候，会将线程持有数据(即线程私有数据)的指针都初始化为NULL,所以需要检查一下
	if(buf == NULL){
		buf = malloc(MAX_ERROR_LEN);
		if(buf == NULL)
			errExit("malloc");

		s = pthread_setspecific(strerrorKey, buf);	//将key与本线程的buf关联起来，当线程结束时，会将buf作为参数传递给destructor
		if(s != 0)
			errExitEN(s, "pthread_setspecific");

	}

	if(err < 0 || err > _sys_nerr || _sys_errlist[err] == NULL){
	       snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
	 }else{
  		strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
		buf[MAX_ERROR_LEN - 1] = '\0';
	 }

	return buf;
}	

