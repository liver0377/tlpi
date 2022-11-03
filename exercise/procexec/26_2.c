#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../lib/tlpi_hdr.h"

int main(int argc, const char *argv[]){
    
    setbuf(stdout, NULL);

    int age = 0; 

    
    switch(fork()){
        case -1: errExit("fork");
        case  0: age = 0; break;
        default: age = 1; break;
    }
    if(age == 0){
        switch(fork()){
            case -1: errExit("fork");
            case  0: age = 0; break;
            default: age = 1; break;
        }
    }else{
        age += 1;
    }
    printf("I am PID %ld, my age is %d\n", (long)getpid(), age);

    switch(age){
        case 2: sleep(2); break;            // grandpa go to sleep plz
        case 1: _exit(EXIT_SUCCESS); break; // father commit suicide
        case 0: sleep(2); break;            // Stay there, I will call the med.
    }
    printf("I am PID %ld, my age is %d, my father is %ld\n", (long)getpid(), age, (long)getppid());
    return 0;
}
