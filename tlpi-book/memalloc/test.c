#include<stdio.h>
int main(int argc, char** argv){
	int* p = (int*)malloc(10);
	printf("p is: %p\n", p);
	return 0;
}
