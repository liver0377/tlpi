#include<pwd.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char** argv){
	if(argc != 2 || argv[1] == "--help")
		usageErr("%s name",argv[0]);
	struct passwd* pwd;

	while((pwd = getpwent()) != NULL){
		if(strcmp(argv[1], pwd->pw_name) ==0){
		printf("the id of %s is: %d\n", argv[1], pwd->pw_uid);
		break;
		}
	}
	
	endpwent();
	if(pwd == NULL)
	  errExit("getpwent");
	
	exit(EXIT_SUCCESS);
}	
