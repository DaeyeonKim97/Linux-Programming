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


	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { //똑같은 key를 이용해서 shared memory get
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  { //shmid를 이용해서 sharedmemory attach
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr; //flag
	pData = ptr + sizeof(int); //data를 가르킴
	sprintf(pData, "This is a request from sipc2 %d.", getpid());
	*pInt = 1;
	printf("Sent a request.....");

	while ((*pInt) == 1) //flag가 1이 될때 (쓰여졌을때) loop탈출
		;

	printf("Received reply: %s\n", pData); //data출력
}
