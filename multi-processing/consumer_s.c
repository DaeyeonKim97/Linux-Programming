#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"
//
main()
{
	BoundedBufferType	*pBuf;
	int			shmid, i, data;
	int			emptySemid, fullSemid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { //prodcons.h의 환경변수를 이용하여 shm생성
		perror("shmget");
		exit(1); //실패 시 perror shmget, 프로세스 종료
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  { //shmid를 attach, 주소를 pBuf에 저장
		perror("shmat");
		exit(1); //실패 시 perror shmat
	}

	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  { //empty세마포어를 만듦
		fprintf(stderr, "semInit failure\n");
		exit(1); //실패 시 종료
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  { //full세마포어를 만듦
		fprintf(stderr, "semInit failure\n");
		exit(1); //실패 시 종료
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  { //mutex세마포어를 만듦
		fprintf(stderr, "semInit failure\n");
		exit(1); //실패 시 종료
	}

	if (semInitValue(emptySemid, MAX_BUF) < 0)  { //empty세마포어의 개수를 buffer의 최대크기로 초기화
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(fullSemid, 0) < 0)  { //full세마포어의 개수를 0으로 초기화
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(mutexSemid,1) < 0)  { //mutex세마포어의 개수를 1로 초기화
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	srand(0x9999);

	for (i = 0 ; i < NLOOPS ; i++)  {
		if (semWait(fullSemid) < 0)  { //full 세마포어 wait
			fprintf(stderr, "semWait failure\n");
			exit(1); //실패 시 종료
		}
		if (semWait(mutexSemid) < 0)  { //mutex 세마포어 wait
			fprintf(stderr, "semWait failure\n");
			exit(1); //실패 시 종료
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data; //buffer의 내용을 data에 받아옴
		pBuf->out = (pBuf->out + 1) % MAX_BUF; //out index 최신화
		pBuf->counter--; //buffer의 카운터를 1개 낮춤

		if (semPost(mutexSemid) < 0)  { //mutex세마포에어 signal
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if (semPost(emptySemid) < 0)  { //empty세마포어에 signal
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand()%100)*10000); //랜덤시간동안 sleep
	}

	printf("Consumer: Consumed %d items.....\n", i); //몇개의 아이템을 consume했는지 출력
	printf("Consumer: %d items in buffer.....\n", pBuf->counter); //몇개의 아이템이 버퍼에 남았는지 출력
}
