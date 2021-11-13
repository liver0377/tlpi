#include<unistd.h>
#include<stdio.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>

#define MAX_CMD_LEN 200
int system(const char *command){
	sigset_t blockMask, originMask;
	struct sigaction saIgnore, saOriginQuit, saOriginInt, saDefault;
	pid_t childPid;
	int status, savedErrno;

	//当命令行为空时，shell如果可用则返回0,不可用则返回-1,通过对内置命令":"的调用结果来进行判断
	if(command == NULL)
		return system(":") == 0;

	//提前阻塞SIGCHLD信号,以防下面的情况
	//如果父进程除了system创建的shell子进程之外还创建了其他子进程，并且自身的SIGCHLD信号处理函数中也调用了wait
	//此时就有可能产生竞争条件，导致信号处理函数比system函数优先获取到子进程的终止状态
	//而且这还有可能会导致误判，system创建的子进程产生的SIGCHLD信号错误地被一开始的父进程所捕获，这并非信号处理函数的本意,信号处理函数应该是用来捕获自己	    //手动使用fork创建出来的子进程
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockMask, &originMask);

	//忽略SIGINT和SIGQUIT信号，因为父进程与shell进程以及shell创建的命令子进程统统位于同一个进程组，使用SIGINT和SIGQUIT命令会同时对所有的这些进程痛处发出	    //信号，对这些信号进行处置意义不大
	saIgnore.sa_handler = SIG_IGN;
	saIgnore.sa_flags = 0;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT, &saIgnore, &saOriginInt);
	sigaction(SIGQUIT, &saIgnore, &saOriginQuit);

	switch(childPid = fork()){
		case -1:
			status = -1;
			break;
		case 0:
			//创建出的子shell进程，准备执行命令
			saDefault.sa_handler = SIG_DFL;
			saDefault.sa_flags = 0;
			sigemptyset(&saDefault.sa_mask);

			//根据Susv3规定，当使用sysrem时就如同调用fork + exec,而fork是不会改变信号处置
			//对于exec,如果信号处置为SIG_EFL或者SIG_IGN,那么他们的信号处置不变，否则统统置为SIG_DEL
			if(saOriginInt.sa_handler != SIG_IGN)
				sigaction(SIGINT, &saDefault, NULL);
			if(saOriginQuit.sa_handler != SIG_IGN)
				sigaction(SIGQUIT, &saDefault, NULL);

			sigprocmask(SIG_SETMASK, &originMask, NULL);
			execl("/bin/sh", "sh", "-c", command, (char *) NULL);
			_exit(127);		//shell子进程不能够执行命令，返回127

		default:
			while(waitpid(childPid, &status, 0) == -1){
				if(errno != EINTR){
					status = -1;
					break;
				}
			}
			break;
	}

	//创建完子进程并且完成了命令的执行，恢复对于信号的处置
	savedErrno = errno;

	sigprocmask(SIG_SETMASK, &originMask, NULL);
	sigaction(SIGINT, &saOriginInt, NULL);
	sigaction(SIGQUIT, &saOriginQuit, NULL);

	errno = savedErrno;

	return status;
}


int main(int argc, char **argv){
	char str[MAX_CMD_LEN];
	int status;

	for(;;){
		printf("Command: ");
		fflush(stdout);
		if(fgets(str, MAX_CMD_LEN, stdin) == NULL)
			break;

		status = system(str);
	}
}
