#include<mqueue.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	if(mq_open("/myobject", O_CREAT, O_RDONLY, NULL) == (mqd_t) -1){
		printf("%s", strerror(errno));
			errExit("mq_open");
	}
	return  0;
}
