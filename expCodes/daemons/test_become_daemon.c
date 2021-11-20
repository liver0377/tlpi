#include<unistd.h>
#include<stdio.h>
#include"become_daemon.h"

int main(int argc, char **argv){
	becomeDaemon(0);
	sleep(50);
	return 0;
}
