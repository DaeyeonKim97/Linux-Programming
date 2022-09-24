#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE];

main() //shared memory가 virtual memory address상의 어디에 위치하는지?
{
	int		shmid;
	char	*ptr, *shmptr;

	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  { //malloc를 통해 dynamic memory allocation
		perror("malloc");
		exit(1);
	}

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  { //shared memory get을 통해 shared memory id받음
		//IPC_PRIVATE : 이 프로세스가 종료되면 할당했던 physical memory도 알아서 반환됨.
		// parent와 child사이에 shared memory에 쉽게 access
		perror("shmget");
		exit(1);
	}
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  { //shmid로 sharedmemory attach하여 shared memory의 시작주소를 ptr로 넘김
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]); //array의 할당 위치
	printf("Stack around %p\n", &shmid); //stack은 local변수 주변에 저장 (shmid는 stack에 저장됨)
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE); // dynamic allocation malloc은 heap에 저장
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE); //stack과 heap의 중간부분에 저장되어있음

	
	if (shmdt(shmptr) < 0)  {
		perror("shmdt");
		exit(1);
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { //shared memory를 get해서 만든걸 삭제(강조를 위해)
		//IPC private에 의해 삭제됨
		perror("shmctl");
		exit(1);
	}
}
