#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

int main()
{
	BoundedBufferType	*pBuf; //BoundedBufferType의 Shared memory를 저장할 pointer
	int			shmid, i, data;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { //prodcons.h의 환경변수를 이용해 sharedmemory get
		perror("shmget");
		exit(1); //실패 시 perror shmget 출력 후 종료
	}
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1){//shmid에 sharedmemory attach하고 pBuf에 주소
		perror("shmat");
		exit(1); //실패 시 perror shmat 출력 후 종료
	}

	srand(0x9999); //random seed

	for (i = 0 ; i < NLOOPS ; i++)  { //NLOOPS동안 반복
		if (pBuf->counter == 0)  { //Buffer가 비어 있을 경우
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (pBuf->counter == 0) //Buffer에 데이터가 생길 때 까지 waiting
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data; //out번째 index의 데이터를 변수 data에 가져옴
		pBuf->out = (pBuf->out + 1) % MAX_BUF; // out번째 index 최신화
		pBuf->counter--; //counter을 1 줄여줌 

		usleep((rand()%100)*10000); //랜덤시간동안 micro sec sleep
	}

	printf("Consumer: Consumed %d items.....\n", i); //몇개의 items를 소모했는지
	printf("Consumer: %d items in buffer.....\n", pBuf->counter); //몇 개의 item이 남아있는지
}
