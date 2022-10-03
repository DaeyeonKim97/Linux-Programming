#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
pthread_cond_t		NotFull;
pthread_cond_t		NotEmpty;
pthread_mutex_t		Mutex;

void ThreadUsleep(int usecs) // usec 시간동안 Thread sleep 하는 함수
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

	for (i = 0 ; i < NLOOPS ; i++)  { //NLOOPS번 반복
		if (pthread_mutex_lock(&Mutex) < 0)  { //Mutex mutex를 lock
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		while (Buf.counter == MAX_BUF)  { //만약 MAX_BUF갯수가 꽉 차 있을경우
			if (pthread_cond_wait(&NotFull, &Mutex) < 0)  { //NotFull Mutex를 기다림
				perror("pthread_cond_wait");
				pthread_exit(NULL); //실패 시 Thread 종료
			}
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000; //random data 생성
		Buf.buf[Buf.in].data = data; //random data를 Buffer에 넣음
		Buf.in = (Buf.in + 1) % MAX_BUF; //in index 최신화
		Buf.counter++; //counter의 개수 증가

		if (pthread_cond_signal(&NotEmpty) < 0)  { //NotEmpty mutex에 signal을 보냄
			perror("pthread_cond_signal");
			pthread_exit(NULL);//실패 시 thread 종료
		}
		if (pthread_mutex_unlock(&Mutex) < 0)  { //Mutex mutex에 signal을 보냄
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);//실패 시 thread 종료
		}

		ThreadUsleep(data); //Thread를 data micro secs만큼 sleep
	}

	printf("Producer: Produced %d items.....\n", i); //몇개의 아이템을 생성했는지
	printf("Producer: %d items in buffer.....\n", Buf.counter); //몇 개의 아이템이 buf에 있는지
	
	pthread_exit(NULL);
}

void Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  { //NLOOPS번 반복
		if (pthread_mutex_lock(&Mutex) < 0)  { //Mutex mutex lock
			perror("pthread_mutex_lock");
			pthread_exit(NULL); //실패 시 Thread 종료
		}

		while (Buf.counter == 0)  { //Buf에 데이터가 없을 경우
			if (pthread_cond_wait(&NotEmpty, &Mutex) < 0)  { //NotEmpty mutex를 기다림
				perror("pthread_cond_wait");
				pthread_exit(NULL); //실패 시 thread 종료
			}
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; //buffer의 내용을 data에 받아옴
		Buf.out = (Buf.out + 1) % MAX_BUF; //out index 초기화
		Buf.counter--; //buffer counter -1

		if (pthread_cond_signal(&NotFull) < 0)  { //NotFull mutex에 signal을 보냄
			perror("pthread_cond_signal");
			pthread_exit(NULL); //실패 시 thread 종료
		}
		if (pthread_mutex_unlock(&Mutex) < 0)  { //Mutex mutex에 signal을 보냄
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);//실패 시 thread 종료
		}

		ThreadUsleep((rand()%100)*10000); //랜덤시간동안 Thread Usleep
	}

	printf("Consumer: Consumed %d items.....\n", i); //몇 개의 items를 소모했는지
	printf("Consumer: %d items in buffer.....\n", Buf.counter); //버퍼에 몇 개의 items가 남아있는지

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	if (pthread_cond_init(&NotFull, NULL) < 0)  { //NotFull mutex 초기화
		perror("pthread_cond_init");
		pthread_exit(NULL); //실패 시 thread 종료
	}
	if (pthread_cond_init(&NotEmpty, NULL) < 0)  { //NotEmpty mutex 초기화
		perror("pthread_cond_init");
		pthread_exit(NULL); //실패 시 thread 종료
	}
	if (pthread_mutex_init(&Mutex, NULL) < 0)  { //Mutex mutex 초기화
		perror("pthread_mutex_init");
		pthread_exit(NULL); //실패 시 thread 종료
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  { //Producer thread 생성
		perror("pthread_create");
		exit(1); //실패 시 process 종료
	}

	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  { //Consumer thread 생성
		perror("pthread_create");
		exit(1);//실패 시 process 종료
	}

	if (pthread_join(tid1, NULL) < 0)  { //tid1(Producer thread)의 종료를 기다림
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { //tid2(Consumer thread)의 종료를 기다림
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter);  //Buf에 몇개의 아이템이 남아있는지

	if (pthread_cond_destroy(&NotFull) < 0)  { //Notfull mutex destroy
		perror("pthread_cond_destroy");
	}
	if (pthread_cond_destroy(&NotEmpty) < 0)  { //NotEmpty mutex destroy
		perror("pthread_cond_destroy");
	}
	if (pthread_mutex_destroy(&Mutex) < 0)  { //Mutex mutex destroy
		perror("pthread_mutex_destroy");
	}
}
