#include<sys/types.h>
#include<sys/shm.h>
#include"/home/liver0377/tlpi/expCodes/lib/tlpi_hdr.h"

int main(int argc, char **argv){
	struct shmid_ds buf;
	
	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s shared-memory-id", argv[0]);

	int shmid = atoi(argv[1]);
	if (shmctl(shmid, IPC_STAT, &buf) == -1)
		errExit("shmctl");

	printf("shm_perm:\n");
	printf("key: %d\n"
	       "uid: %d\n"
	       "gid: %d\n"
	       "cuid:%d\n"
	       "cgid:%d\n"
	       "mode:%d\n"
	       "__seq:%d\n",buf.shm_perm.__key, buf.shm_perm.uid, buf.shm_perm.gid, buf.shm_perm.cuid, buf.shm_perm.mode, buf.shm_perm.__seq);

	printf("shm_segsz: %ld\n", buf.shm_segsz);
	printf("shm_atime: %ld\n", buf.shm_atime);
	printf("shm_dtime: %ld\n", buf.shm_dtime);
	printf("shm_ctime: %ld\n", buf.shm_ctime);
	printf("shm_cpid:  %d\n", buf.shm_cpid);
	printf("shm_lpid:  %d\n", buf.shm_lpid);

	return 0;
}
