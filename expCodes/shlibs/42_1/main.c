#include<dlfcn.h>
#include<stdio.h>
#include"../../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	void *handle;
	//将两个共享库全部载入
	if((handle = dlopen("./libp1.so.1", RTLD_NOW)) == NULL)
		errExit("dlopen");
	if(dlopen("./libp2.so.1", RTLD_NOW) == NULL)
		errExit("dlopen");

	switch(fork()){
		case -1:
			errExit("fork");
		case 0:
			alarm(3);
			for(;;)
				p2();  //子进程不断使用f2(),f2()不断使用f1()
		default:
			break;
	}	
	
	sleep(1);
	if(dlclose(handle) == -1)
		errExit("dlclose");

	handle = dlopen("./libp1.so.1", RTLD_NOLOAD);
	if(handle == NULL)
		printf("./libp1.so.1正在被使用中，无法卸载\n");
	else
		printf("./libp1.so.1被卸载\n");


	return 0;
}



