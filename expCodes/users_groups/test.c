#include<pwd.h>
#include<grp.h>
#include<stdio.h>
#include"ugid_functions.h"
#include<ctype.h>
#include<limits.h>
#include"../lib/tlpi_hdr.h"


int main(int argc, char** argv){
	int numOfGroups;
	int i;
	gid_t group[NGROUPS_MAX + 1];

	numOfGroups = getgroups(NGROUPS_MAX + 1, group);
	if(numOfGroups == -1)
		errExit("getgroup");

	printf("before initgroups\n");
	for(i = 0; i < numOfGroups; i++){
		printf("%s    (%d);", groupNameFromId(group[i]), group[i]);
	}

	printf("after initgroups\n");

	if(initgroups("liver0377", 60) == -1)
		errExit("initgroups");
	if(seteuid(1000) == -1)
		errExit("seteuid(1000)");

	numOfGroups = getgroups(NGROUPS_MAX + 1, group);
	for(i = 0; i < numOfGroups; i++)
		printf("%s   (%d);", groupNameFromId(group[i]), group[i]);
   	return 0;
}

