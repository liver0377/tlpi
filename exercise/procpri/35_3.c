#define _GNU_SOURCE
#include<sched.h>
#include<sys/times.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	struct sched_param param;
	
	param.sched_priority = 10;
	if(sched_setscheduler(0, SCHED_FIFO, &param) == -1)
	       errExit("sched_setscheduler");

	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(0, &set);		//将当前进程限制在CPU0上，子进程会继承当前进程的CPU亲和力，使得父子进程不会同时运行
	if(sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1)
		errExit("sched_setaffinity");


	if(fork() == -1)
		errExit("fork");

	clock_t cputime;
	struct tms buf;
	clock_t next = 0;		//用于1/4秒计数
	clock_t next_switch = 0;	//用于1秒计数
	for(;;){
		
		if(times(&buf) == (clock_t) -1)
			errExit("times");

		cputime = buf.tms_utime + buf.tms_stime;
		if(cputime == next){
			printf("当前为进程%ld, 所占用的CPU时间为:%lf秒\n", (long) getpid(), (double) cputime / sysconf(_SC_CLK_TCK));
			next += sysconf(_SC_CLK_TCK) / 4;
		}

		if(cputime == next_switch){
			printf("进程%ld跳转\n", (long) getpid());
			next_switch += sysconf(_SC_CLK_TCK);
			sched_yield();
		}
		if(cputime == 3 * sysconf(_SC_CLK_TCK))
			break;
	}
	return 0;
}


