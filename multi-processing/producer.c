#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

int main()
{
	BoundedBufferType	*pBuf;
	int			shmid, i, data;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  { //prodcons.h의 환경변수를 이용하여 shared memory를 만듦
		perror("shmget"); 
		exit(1); //실패 시 perror shmget 출력 후 종료
	}

	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  { //shmid에 shared memory attach, pBuf에 주소
		perror("shmat"); 
		exit(1); //실패 시 perror shmat 출력 후 종료
	}

	srand(0x8888); //srand에 seed

	for (i = 0 ; i < NLOOPS ; i++)  {
		if (pBuf->counter == MAX_BUF)  { //Buf에 데이터가 가득 찼을 경우
			printf("Producer: Buffer full. Waiting.....\n");
			while (pBuf->counter == MAX_BUF) //Buf의 data가 가득 차지 않을 때 까지 대기
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; //random data 생성
		pBuf->buf[pBuf->in].data = data; //Buf.in 번째 인덱스에 data를 넣음
		pBuf->in = (pBuf->in + 1) % MAX_BUF; //Buf.in 인덱스 최신화
		pBuf->counter++; //Buf.counter의 갯수를 늘려줌

		usleep(data); //data micro secs 만큼 sleep
	}

	printf("Producer: Produced %d items.....\n", i); //생성한 items의 갯수

	sleep(2); //2secs 만큼 sleep
	printf("Producer: %d items in buffer.....\n", pBuf->counter); //버퍼의 아이템의 갯수
	
	if (shmctl(shmid, IPC_RMID, 0) < 0)  { //Shared memory shmid 삭제
		perror("shmctl");
		exit(1);
	}
}
