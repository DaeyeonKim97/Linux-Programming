#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"
//
BoundedBufferType Buf;

void ThreadUsleep(int usecs) //usecs단위로 thread를 sleep
{
	pthread_cond_t		cond; //conditional signal 감지
	pthread_mutex_t		mutex; //한번에 하나의 thread만
	struct timespec		ts;
	struct timeval		tv;

	if(pthread_cond_init(&cond, NULL) < 0){ //조건변수cond 초기화, attr이 NULL이면 기본 조건사용
		perror("pthread_cond_init");
		pthread_exit(NULL);
		//실패 시 perror 출력 후 thread 종료
	}

	if(pthread_mutex_init(&mutex, NULL) <0){ //mutex 초기화, NULL이면 default 'fast'사용
		perror("pthread_mutex_init");
		pthread_exit(NULL);
		//실패 시 perror 출력 후 thread 종료
	}

	gettimeofday(&tv, NULL); // 1970-01-01 00::00:00 +0000 (UTC)이후 현재까지 경과된 초와 micro초를 얻음
	ts.tv_sec = tv.tv_sec + usecs/1000000; 
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000){
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if(pthread_mutex_lock(&mutex)<0){ //mutex의 critical section에서 lock
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	if(pthread_cond_timedwait(&cond,&mutex,&ts) <0){ //cond가 참이 될 때까지 wait. 
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if(pthread_cond_destroy(&cond) <0){ // 조건변수의 파괴
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}

	if (pthread_mutex_destroy(&mutex) < 0)  { // mutex에 할당한 리소스 해방
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}


}

void Producer(void *dummy) //BoundedBufferType Buf에 data를 넣는 함수(thread)
{
	int	i, data;
	printf("Producer: Start......\n");

	for(i=0; i<NLOOPS; i++){ //Producer Thread에서 NLOOPS동안 반복
		if(Buf.counter == MAX_BUF){ //MAX_BUF를 넘어가는 data가 들어왔을 경우
			printf("Producer: Buffer full. Waiting.....\n");
			while(Buf.counter == MAX_BUF) //MAX_BUF보다 작아질때까지 대기
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; //data에 10000~990000의 랜덤 값을 넣음
		Buf.buf[Buf.in].data = data; //buf의 data가 들어가는 index에 data를 넣음
		Buf.in = (Buf.in +1) % MAX_BUF; //들어가는 index를 최신화
		Buf.counter++; //counter값 증가

		ThreadUsleep(data); //data micro second만큼 Thread를 sleep
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter); //Producer의 결과를 출력하고 종료

	pthread_exit(NULL);
}

void Consumer(void *dummy) //BoundedBufferType Buf에서 data를 꺼내는 함수(thread)
{
	int	i, data;
	printf("Consumer: Start.....\n");

	for(i=0; i<NLOOPS; i++){ //Counsumer Thread에서 NLOOPS동안 반복
		if(Buf.counter == 0){ //Buf에 data가 없을 경우
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (Buf.counter == 0) //data가 들어올 때 까지 대기
				;
		}

		printf("Consumer: Consuming an item.....\n"); 
		data = Buf.buf[Buf.out].data; // 소모할 index의 data를 받아옴
		Buf.out = (Buf.out +1) % MAX_BUF; // out index를 최신화
		Buf.counter--; //counter값 감소

		ThreadUsleep((rand()%100) * 10000); // 랜덤시간동안 sleep
	}

	printf("Consumer: Consumed %d items.....\n",i); //Consumer의 결과를 출력하고 종료
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888); //rand seed값

	if(pthread_create(&tid1, NULL, (void*)Producer, (void*)NULL) <0){ //Producer를 실행하는 pthread create
		perror("pthread_create");
		exit(1);
	}

	if(pthread_create(&tid2, NULL, (void*)Consumer, (void*)NULL) <0){ //Consumer를 실행하는 pthread create
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tid1, NULL) < 0){ //tid1의 종료를 기다림
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) <0){ //tid2의 종료를 기다림
		perror("pthread_join");
		exit(1);
	}

	printf("Main	: %d items in buffer.....\n", Buf.counter); //Buf에 있는 items의 개수를 출력 후 종료

}
