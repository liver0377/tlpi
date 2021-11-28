#include <sys/mman.h>
#include "../../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	void *addr;
	addr = mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	int *p = (int *)addr;
	*p = 1;

	if (madvise(addr, 1000, MADV_DONTNEED) == -1)
		errExit("madvise");

	printf("*p is :%d\n", *p);
	return 0;
}
