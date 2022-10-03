#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "semlib2.h"
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t			EmptySem, FullSem, MutexSem;

void ThreadUsleep(int usecs) //Thread를 usecs만큼 sleep하는 함수
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void Producer(void *dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
		if (sem_wait(&EmptySem)<0)  { //Empty세마포어를 wait
			perror("sem_wait");
			pthread_exit(NULL); //실패 시 thread 종료
		}
		if (sem_wait(&MutexSem)<0)  { //Mutex세마포어를 wait
			perror("sem_wait");
			pthread_exit(NULL); //실패 시 thread 종료
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; //random data생성
		Buf.buf[Buf.in].data = data; //buffer에 data추가
		Buf.in = (Buf.in + 1) % MAX_BUF; //in index 최신화
		Buf.counter++; //buf counter증가

		if (sem_post(&MutexSem)<0)  { //Mutex세마포어에 signal
			perror("sem_post");
			pthread_exit(NULL); //실패 시 thread 종료
		}
		if (sem_post(&FullSem)<0)  { //Full 세마포어에 singal
			perror("sem_post");
			pthread_exit(NULL); //실패 시 thread 종료
		}

		ThreadUsleep(data); //Thread sleep
	}

	printf("Producer: Produced %d items.....\n", i); //몇개의 items를 produce했는지
	printf("Producer: %d items in buffer.....\n", Buf.counter); //몇개의 items가 buffer에 남아있는지
	
	pthread_exit(NULL); //thread 종료
}

void
Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  { //NLOOPS번 반복
		if (sem_wait(&FullSem)<0)  { //Full semaphore wait
			perror("sem_wait");
			pthread_exit(NULL); //thread exit with failure
		}
		if (sem_wait(&MutexSem)<0)  { //Mutex semaphore wait
			perror("sem_wait");
			pthread_exit(NULL); //thread exit with failure
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; //buf의 data를 받아옴
		Buf.out = (Buf.out + 1) % MAX_BUF; //out index 최신화
		Buf.counter--; //buf의 counter -1

		if (sem_post(&MutexSem)<0)  { //Mutex semaphore signal
			perror("sem_post");
			pthread_exit(NULL); //thread exit with failure
		}
		if (sem_post(&EmptySem)<0)  { //Empty semphore signal
			perror("sem_post");
			pthread_exit(NULL); //thread exit with failure
		}

		ThreadUsleep((rand()%100)*10000); //random time thread sleep
	}

	printf("Consumer: Consumed %d items.....\n", i); //몇 개의 아이템을 소비했는지
	printf("Consumer: %d items in buffer.....\n", Buf.counter); //지금 버퍼에 ㅁㅕㅊ개의 아이템이 있는지

	pthread_exit(NULL); //thread 종료
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	if (sem_init(&EmptySem, 0, MAX_BUF)<0)  { //Empty semaphore를 MAX_BUF개로 초기화
		perror("sem_init");
		exit(1); //실패 시 프로세스 종료
	}
	if (sem_init(&FullSem,0,0)<0)  { //Full semaphore를 0개로 초기화
		perror("sem_init");
		exit(1); //실패 시 프로세스 종료
	}
	if (sem_init(&MutexSem,0,1)<0)  { //Mutex semaphore를 1개로 초기화
		perror("sem_init");
		exit(1); //실패 시 프로세스 종료
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  { //Producer thread create
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  { //Consumer thread create
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  { //tid1의 종료를 기다림
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { //tid2의 종료를 기다림
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter);

	if (sem_destroy(&EmptySem<0))  { //Empty semaphore destroy
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem)<0)  { //Full semaphore destroy
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem)<0)  { //destroy Mutex semaphore
		perror("sem_destroy");
	}
}
