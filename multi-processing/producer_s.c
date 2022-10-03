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

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { //prodcons.h의 환경병수를 이용하여 SHM생성
		perror("shmget");
		exit(1); //실패 시 perror shmget, 프로세스 종료
	}

	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  { //shmid에 attach, pBuf에 주소 저장
		perror("shmat");
		exit(1); //실패 시 perror shmat, 프로세스 종료
	}

	if ((emptySemid = semInit(EMPTY_SEM_KEY) ) < 0)  { //환경변수 EMPTY_SEM_KEY를 이용하여 세마포어생성
		fprintf(stderr, "semInit failure\n");
		exit(1); //실패 시 stderr semInit failure, 프로세스 종료
	}
	if ((fullSemid = semInit(FULL_SEM_KEY) ) < 0)  { //환경변수 FULL_SEM_KEY를 이용하여 세마포어생성
		fprintf(stderr, "semInit failure\n");
		exit(1); //실패 시 stderr semInit failure, 프로세스 종료
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY) ) < 0)  { //환경변수 MUTEX_SEM_KEY를 이용하여 세마포어생성
		fprintf(stderr, "semInit failure\n");
		exit(1); //실패 시 stderr semInit failure, 프로세스 종료
	}

	srand(0x8888); //random seed

	for (i = 0 ; i < NLOOPS ; i++)  { //NLOOPS번 반복
		if (semWait(emptySemid) < 0)  { //empty세마포어의 자리가 나올 때 까지 대기(비어있는지?)
			fprintf(stderr, "semWait failure\n");
			exit(1); //실패 시 stderr semWait failure, 프로세스 종료
		}
		if (semWait(mutexSemid) < 0)  { //mutex세마포어의 자리가 나올 때 까지 대기(critical section)
			fprintf(stderr, "semWait failure\n");
			exit(1); //실패 시 stderr sesmWait failure, 프로세스 종료
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; //랜덤 데이터 생성
		pBuf->buf[pBuf->in].data = data; //데이터를 버퍼에 할당
		pBuf->in = (pBuf->in + 1) % MAX_BUF; //in index 최신화
		pBuf->counter++; //counter의 개수 증가

		if (semPost(mutexSemid) < 0)  { //mutex세마포어에 signal
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if (semPost(fullSemid) < 0)  { //full세마포어에 signal
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep(data); //data micro secs만큼 sleep
	}

	printf("Producer: Produced %d items.....\n", i); //몇개의 item을 produce했는지 출력

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter); //몇개의 아이템이 buf에 있는지 출력
	
	if (semDestroy(emptySemid) < 0)  { //empty세마포어 제거
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0)  { //full세마포어 제거
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0)  { //mutex세마포어 제거
		fprintf(stderr, "semDestroy failure\n");
	}
	if (shmctl(shmid, IPC_RMID, 0) < 0)  { //BoundedBufferType으로 사용했던 sharedmemory제거
		perror("shmctl");
		exit(1);
	}
}
