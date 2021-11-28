#define _BSD_SOURCE
#include <sys/resource.h>
#include <sys/mman.h>
#include "../../lib/tlpi_hdr.h"
//经过测验好像没有问题发生
int main(int argc, char **argv){
        //调整RLIMIT_MEMLOCK
        struct rlimit orirlim, currlim;
        int s;
        s = getrlimit(RLIMIT_MEMLOCK, &orirlim);
        if (s == -1)
                errExit("getrlimit");

        currlim.rlim_cur = orirlim.rlim_cur - 10000;
        currlim.rlim_max = orirlim.rlim_max;

        s = setrlimit(RLIMIT_MEMLOCK, &currlim);
        if (s == -1)
                errExit("setrlimit");

        //创建私有匿名映射
        void *addr;
        addr = mmap(NULL, orirlim.rlim_cur, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (addr == MAP_FAILED)
                errExit("mmap");

        //将超过RLIMIT_MEMLOCK的字节数锁进内存
        if (mlock(addr, orirlim.rlim_cur) == -1)
                errExit("mlock");

        //可能不会执行到这
        //解除锁定
        if (munlock(addr, orirlim.rlim_cur) == -1)
                errExit("munlock");

        //解除映射
        if (munmap(addr, orirlim.rlim_cur) == -1)
                errExit("munmap");

        //恢复限制
        if (setrlimit(RLIMIT_MEMLOCK, &orirlim) == -1)
                errExit("setrlimit");
        return 0;
}
