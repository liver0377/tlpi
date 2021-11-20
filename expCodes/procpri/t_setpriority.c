#include<sys/time.h>
#include<sys/resource.h>
#include"../lib/tlpi_hdr.h"

int main(int argc, char **argv){
	int which, prio;
	id_t who;

	if(argc != 4 || strchr("pgu", argv[1][0]) == NULL)
		usageErr("%s {p|g|u} who prio]\n",
			 "set priority of: p = progress; g = progerss group; u = real user\n", argv[0]);

	which = (argv[1][0] == 'p') ? PRIO_PROCESS :
		(argv[1][0] == 'g') ? PRIO_PGRP	    :
		PRIO_USER;

	prio = getInt(argv[3], 0, "prio");
	
	if(setpriority(which, who, prio) == -1)
		errExit("setpriority");

	errno = 0;
	if((prio = getpriority(which, who)) == -1 && errno != 0)
		errExit("getpriority");

	printf("Nice value is : %d\n", prio);
	return 0;
}
