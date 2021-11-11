#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int istack = 222;

	switch(vfork()){
		case -1:
			errExit("vfork");
		case 0:
			sleep(3);		//尽管sleep了三秒，但依旧是子进程先执行，因为vfork使得父进程被挂起
			write(STDOUT_FILENO, "child executing\n", 16);
			istack *= 3;
			_exit(EXIT_SUCCESS);
		default:
			write(STDOUT_FILENO, "parent execting\n", 16);
			printf("istack: %d", istack);
			exit(EXIT_SUCCESS);
	}
}
