#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../../lib/tlpi_hdr.h"
//将源文件和目标文件均映射到当前进程的地址空间，然后直接在当前地址空间上对文件进行操作即可

int getSize(int fd){
	struct stat statbuf;
	if (fstat(fd, &statbuf) == -1)
		errExit("fstat");

	return statbuf.st_size;
}

int main(int argc, char **argv){
	if (argc != 3 || strcmp (argv[1], "--help") == 0)
		usageErr("%s src dst", argv[0]);
	
	umask(0);	
	int src = open(argv[1], O_RDONLY, O_RDONLY);
	if (src == -1)
		fatal("file does't exist");

	int dst = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if (dst == -1)
		errExit("open");

	//设置输出文件大小
        int size = getSize(src);	
	if (ftruncate(dst, size) == -1)
		errExit("ftruncate");

	//将文件映射到当前虚拟空间
	int *addrSrc, *addrDst;
	
	addrSrc = mmap(NULL, size, PROT_READ, MAP_SHARED, src, 0);
	if (addrSrc == MAP_FAILED)
		errExit("mmap");
	
	addrDst = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, dst, 0);
	if (addrDst == MAP_FAILED)
		errExit("addrDst");
	
	//关闭不再使用的文件描述符
	if (close(src) == -1)
		errExit("close");
	if (close(dst) == -1)
		errExit("close");
	void *buf = malloc(size);
	memcpy(addrDst, addrSrc, size);

	if (msync(addrDst, size, MS_SYNC) == -1)
		errExit("msync");
	return 0;
	
}
