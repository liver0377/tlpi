#include<stdio.h>
#include"memalloc.h"

int main(int argc, char** argv){
	int* p = (void*) malloc(10);
	printf("p:  %p\n",p);
	free(p);
	return 0;
}
