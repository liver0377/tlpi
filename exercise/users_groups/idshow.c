#define _GNU_SOURCE
#include<unistd.h>
#include<sys/fsuid.h>
#include<limits.h>
#include"ugid_functions.h"
#include"../lib/tlpi_hdr.h"

//NGROUP_MAX宏定义在<limits.h>头文件中，为进程属组的最大数量
#define SG_SIZE (NGROUPS_MAX + 1)

int main(int argc, char** argv){
	uid_t ruid, euid, suid, fsuid;
	gid_t rgid, egid, sgid, fsgid;
	gid_t suppGroups[SG_SIZE];
	int numGroups, j;
	char *p;

	//获取实际、有效、保存ID
	if(getresuid(&ruid, &euid, &suid) == -1)
		errExit("getresuid");
	if(getresgid(&rgid, &egid, &sgid) == -1)
		errExit("getresgid");

	//获取当前文件系统ID
	fsuid = setfsuid(0);
	fsgid = setfsgid(0);  /*对于非特权进程而言，上述操作并不会修改文件系统ID*/

	printf("UID: ");
	p = userNameFromId(ruid);
	printf("real=%s (%ld);", (p == NULL) ? "???" : p, (long) ruid);
	p = userNameFromId(euid);
	printf("eff=%s (%ld);", (p == NULL) ? "???" : p, (long) euid);
	p = userNameFromId(suid);
	printf("saved=%s (%ld);", (p == NULL) ? "???" : p, (long) suid);
	p = userNameFromId(fsuid);
	printf("fs=%s (%ld); ",(p == NULL) ? "???" :p, (long) fsuid);
	printf("\n");

	printf("GID");
	p = groupNameFromId(rgid);
	printf("real=%s (%ld);", (p == NULL) ? "???" : p, (long) rgid);
	p = groupNameFromId(euid);
	printf("eff=%s (%ld);", (p == NULL) ? "???" : p, (long) egid);
	p = groupNameFromId(suid);
	printf("saved=%s (%ld);", (p == NULL) ? "???" : p, (long) sgid);
	p = groupNameFromId(fsuid);
	printf("fs=%s (%ld); ",(p == NULL) ? "???" :p, (long) fsgid);
	printf("\n");


	numGroups = getgroups(SG_SIZE, suppGroups);
	if(numGroups == -1)
		errExit("getgroups");

	printf("Supplementary groups (%d): ", numGroups);
	for(j = 0; j < numGroups; j++){
		p = groupNameFromId(suppGroups[j]);
		printf("%s (%ld) ", (p ==NULL) ? "???" : p, (long) suppGroups[j]);
	}

	printf("\n");

	exit(EXIT_SUCCESS);
}
