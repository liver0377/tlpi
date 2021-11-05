#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"../lib/tlpi_hdr.h"


extern char** environ;

int unsetenv(const char* name){
size_t len = strlen(name);

    for (char **ep = environ; *ep != NULL; )
        if (strncmp(*ep, name, len) == 0 && (*ep)[len] == '=') {

            /* Remove found entry by shifting all successive entries
               back one element */

            for (char **sp = ep; *sp != NULL; sp++)
                *sp = *(sp + 1);

            /* Continue around the loop to further instances of 'name' */

        } else {
            ep++;
        }

    return 0;
}


int main(int argc, char** argv){
	if(argc != 2 || strcmp(argv[1], "--help") ==0)
		usageErr("%s valuename",argv[0]);
	for(char** ep = environ;*ep!=NULL;ep++)
		puts(*ep);
	unsetenv(argv[1]);
	for(char** ep = environ;*ep!=NULL;ep++)
		puts(*ep);
	exit(EXIT_SUCCESS);

    
}
		
