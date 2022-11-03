#include<setjmp.h>
#include<stdlib.h>
#include<stdio.h>

static jmp_buf env;

void f1(){
	printf("f1() begin !\n");
	setjmp(env);
	printf("f1() end() !\n");
}

	

int main(int argc,char** argv){
	f1();
	printf("ready to longjmp\n");
	longjmp(env,1);
	printf("jump is over\n");
	exit(EXIT_SUCCESS);
}
