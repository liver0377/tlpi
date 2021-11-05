#include<sys/xattr.h>
#include"../lib/tlpi_hdr.h"



static void usageError(char *progName){
	fprintf(stderr, "%s [-n  name [-v value]] pathname", progName);
        return ;
}

int main(int argc, char**argv){
	int opt;
	const char* pathname;
	const char* name;
	const char* value;
	int isDelet = 0;
	pathname = argv[argc - 1];
	while((opt = getopt(argc, argv, "n:v:x:")) != -1){
		//printf("opt: is %c \n", opt);
		switch(opt){
			case 'n': name = argv[optind - 1];                   break; 
			case 'x': name = argv[optind - 1];      isDelet = 1; break;
			case 'v': value = argv[optind - 1];     	     break;
			case '?': usageError(argv[0]);          	     break;
		}
	}

	printf("name: %s  vaue: %s\n", name, value);
	printf("pathnam: %s\n", pathname);
	if(isDelet){
		if(removexattr(pathname, name) == -1)
			errExit("removexattr");
	}

	if(value == NULL){
		if(setxattr(pathname, name, " ", 2, 0) == -1)
			errExit("setxattr");
	}
	else{
		if(setxattr(pathname, name, value, strlen(value), 0) == -1)
			errExit("setxattr");
	}

	return 0;
}
			
