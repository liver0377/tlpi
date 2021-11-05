#define _BSD_SOURCE
#include<stdio.h>
#include<grp.h>
#include<unistd.h>
#include"ugid_functions.h"
#include<limits.h>
#include"../lib/tlpi_hdr.h"

#define GRPSIZE NGROUPS_MAX + 1
int main(int argc, char** argv){
	if(argc != 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s grouop", argv[0]);
	
	gid_t group[GRPSIZE];
	int numOfGroup;
	int i;

	numOfGroup = getgroups(GRPSIZE, group);
	if(numOfGroup == -1)
		errExit("getGruops");

	printf("the old grouoplist:\n");
	for(i = 0; i < numOfGroup; i++){
		printf("%s (%d);", groupNameFromId(group[i]), group[i]);
	}
	printf("\n");
	//添加新加上的一个group
	group[numOfGroup] = groupIdFromName(argv[1]);

	if(setgroups(numOfGroup + 1, group) == -1)
		errExit("setgroup");

	//设置完毕，重新打印grouop列表
	numOfGroup = getgroups(GRPSIZE, group);
	if(numOfGroup == -1)
		errExit("getgroup");
	
	printf("the new grouplist\n");
	for(i = 0; i < numOfGroup; i++){
		printf("%s (%d);", groupNameFromId(group[i]), group[i]);
	}

	printf("\n");
	exit(EXIT_SUCCESS);
}	


