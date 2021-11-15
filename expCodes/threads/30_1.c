#include<pthread.h>
#include"../lib/tlpi_hdr.h"

static int glob = 0;
struct threads{
	int flags;			//线程标识符
	int loops;			//循环次数
}; 

static void* threadFunc(void *arg){
	int loops = ((struct threads *)arg)->loops;
	int loc;

	for(int i = 0; i < loops; i++){
		loc = glob;
		loc++;
		glob++;
		printf("I am thread%d, glob is %d now\n", ((struct threads *)arg)->flags, glob); 
	}
}

int main(int argc, char **argv){
	int s;
	int loops;
	pthread_t t1, t2;
	struct threads s1, s2;

	s1.loops = (argc > 1) ? atoi(argv[1]) : 1000000;
	s2.loops = (argc > 1) ? atoi(argv[1]) : 1000000;
	s1.flags = 1;
	s2.flags = 2;

	s = pthread_create(&t1, NULL, threadFunc, &s1);
	if(s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_create(&t2, NULL, threadFunc, &s2);
	if(s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_join(t1, NULL);
	if(s != 0)
		errExitEN(s, "pthread_join");

	s = pthread_join(t2, NULL);
	if(s != 0)
		errExitEN(s, "pthread_join");

	printf("the program is over, glob is %d now \n", glob);

	return 0;
}
