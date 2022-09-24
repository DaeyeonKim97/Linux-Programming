#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"


main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;

	/* SHM_KEY, SHM_SIZE, SHM_MODE in shm.h */
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0)  { //shared memory가 없으므로 SHM_KEY로 만듦
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid,0,0)) == (void *) -1)  { //shmid로 attach
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr;
	while ((*pInt) == 0) // flag 4byte를 읽음 flag가 0일때는 계쏙 loop를 돔
		;
	
	pData = ptr + sizeof(int); //ptr의 처음 4byte는 flag, 나머지 data
	printf("Received request: %s.....", pData); 
	sprintf(pData, "This is a reply from sipc1 %d.", getpid());
	*pInt = 0;
	printf("Replied.\n");

	sleep(1);


	/* Remove shared memory */
	if (shmctl(shmid, IPC_RMID,0) <0)  {
		perror("shmctl");
		exit(1);
	}
}
