#define _GNU_SOURCE
#include<pthread.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"../lib/tlpi_hdr.h"

#define PATH_LEN
static pthread_key_t key;
static pthread_once_t once = PTHREAD_ONCE_INIT;

void destructor(void * buf){
	free(buf);
}

void init(void){
	int s;

	s = pthread_key_create(&key, destructor); 
	if(s != 0)
		errExitEN(s, "pthread_key_create");
}

//dirname和basename的线程安全版
static char *dirname(char *pathname){
	char *buf;
	int s;

	s = pthread_once(&once, init);
	if(s != 0)
		errExitEN(s, "pthread_once");

	buf = pthread_getspecific(key);
	if(buf == NULL){
		buf = malloc(strlen(pathname));
		if(buf == NULL)
			errExit("malloc");

		s = pthread_setspecific(key, buf);
		if(s != 0)
			errExitEN(s, "pthread_setspecific");

	}

	int len = 0;
	for(int i = 0; i < strlen(pathname); i++){
		if(!(pathname[i] == '/' && pathname[i + 1] == '/'))
			buf[len++] = pathname[i];
	}
	buf[len] = '\0';
	if(buf[len-1] == '/') buf[--len] = '\0';

    	char* c = strrchr(buf,'/');
        if(c != NULL && len > 1) *c = '\0';
    	if(c == NULL) return ".";
    	return buf;
}

static char *basname(char *pathname){
	char *buf;
	int s;

	s = pthread_once(&once, init);
	if(s != 0)
		errExitEN(s, "pthread_once");

	buf = pthread_getspecific(key);
	if(buf == NULL){
		buf = malloc(strlen(pathname));
		if(buf == NULL)
			errExit("malloc");

		s = pthread_setspecific(key, buf);
		if(s != 0)
			errExitEN(s, "ptread_setspecific");
	}

	int len;
	for(int i = 0; i < strlen(pathname); i++){
		if(!(pathname[i] == '/' && pathname[i + 1] == '/'))
			buf[len++] = pathname[i];
	}
	buf[len] = '\0';

	if(buf[len-1] == '/') buf[--len] = '\0';

    	char* c = strrchr(buf,'/');
    	if(c != NULL && len > 1)
        return c+1;

        return buf;
}
		
int main(int argc, char **argv){
	char *pathname = (argc > 1) ? argv[1] : argv[0];

	printf("dirname: %s  basename: %s \n", dirname(pathname), basname(pathname));

	return 0;
}
