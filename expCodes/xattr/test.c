#include<sys/xattr.h>
#include"../lib/tlpi_hdr.h"

static void usageError(char *progName){
        fprintf(stderr, "%s [-n  name [-v value]] pathname", progName);
        return ;
}

int main(int argc, char **argv){
 int opt;
        const char* pathname;
        const char* name;
        const char* value;
        int isDelet = 0;
        while((opt = getopt(argc, argv, "n:v:x:")) != -1){
                printf("opt: is c:%c d: %d \n", opt, opt);
		printf("optarg is:%s\n", optarg);
		printf("optind is:%d\n", optind);
		if(opt == 'n') printf("opt is n now\n");
		if(opt == 'v') printf("opt is v now\n");
                switch(opt){
                        case 'n': name = argv[optind - 1];                   break;
                        case 'x': pathname = argv[optind];  isDelet = 1; break;
                        case 'v': value = argv[optind - 1];                  break;
                        case '?': usageError(argv[0]);                       break;
                }
        }	

	 printf("name: %s  vaue: %s\n", name, value);
        printf("pathnam: %s\n", pathname);
        if(isDelet){
                if(removexattr(pathname, name) == -1)
                        errExit("removexattr");
        }
	exit(EXIT_SUCCESS);
}
