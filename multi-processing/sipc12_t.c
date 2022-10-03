#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include "shm.h"

char    data[256];
/*
    아래 구현한 방법은 전역변수를 semaphore을 사용하여 메시지를 주고받는 방식이다.

    전역변수를 semaphore을 이용하여 메시지를 전달 받으면 메시지를 전달할 때 semaphore의 갯수에 따라 여러 쓰레드가 critical section인 전역변수에 접근할 수 있다. (하지만 아래 코드와 같은 경우는 binary semaphore을 이용한 mutex를 구현하여 critical section에는 하나의 쓰레드만 접근 가능)

    반면 mutex lock의 경우는 한번에 하나의 thread만 접근가능한 영역을 만든다. 따라서 이는 conditional variable의 signal을 보내는 것을 통해 mutex의 lock과 unlock을 결정해야 한다.

*/

sem_t   EmptySem, FullSem, MutexSem;

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

void sipc1(void *dummy)
{
	printf("Sipc1: Start.....\n");

	if (sem_wait(&FullSem)<0)  { //Full 세마포어를 wait, Sipc2의 데이터 전달을 기다림
        perror("sem_wait");
        pthread_exit(NULL); //실패 시 thread 종료
    }

    if (sem_wait(&MutexSem)<0)  { //Mutex세마포어를 wait
        perror("sem_wait");
        pthread_exit(NULL); //실패 시 thread 종료
    }

    printf("(Sipc1: %d)Received request: %s\n", pthread_self(), data); //Sipc2에서 온 데이터를 출력

    *data = '\0'; //data 내용 초기화
	sprintf(data, "This is a reply from s1: %d", pthread_self()); //Sipc1의 데이터를 씀

    if (sem_post(&MutexSem)<0)  { //Mutex세마포어에 signal
        perror("sem_post");
        pthread_exit(NULL); //실패 시 thread 종료
    }

    if (sem_post(&EmptySem)<0)  { //Empty 세마포어에 singal
        perror("sem_post");
        pthread_exit(NULL); //실패 시 thread 종료
    }


    ThreadUsleep((rand()%100)*10000);
	

	pthread_exit(NULL);


	printf("(Sipc1)Replied.\n");
}

void sipc2(void *dummy)
{
	printf("Sipc2: Start.....\n");


    if (sem_wait(&MutexSem)<0)  { //Mutex세마포어를 wait
        perror("sem_wait");
        pthread_exit(NULL); //실패 시 thread 종료
    }

    *data = '\0'; //data 내용 초기화
    sprintf(data, "This is a request from s2: %d", pthread_self()); //Sipc2의 데이터를 씀

    if (sem_post(&MutexSem)<0)  { //Mutex세마포어에 signal
        perror("sem_post");
        pthread_exit(NULL); //실패 시 thread 종료
    }
    
    if (sem_post(&FullSem)<0)  { //Full 세마포어에 singal
        perror("sem_post");
        pthread_exit(NULL); //실패 시 thread 종료
    }

    printf("(Sipc2 :%d)Sent a request.....\n",pthread_self()); 

    if (sem_wait(&EmptySem)<0)  { //Empty세마포어를 wait, Sipc1의 데이터를 기다림
        perror("sem_wait");
        pthread_exit(NULL); //실패 시 thread 종료
    }


    printf("(Sipc2 :%d)Received reply: %s\n", pthread_self(), data); // Sipc1에서 받은 데이터터를 출력


    ThreadUsleep((rand()%100)*10000);

	
	pthread_exit(NULL);
}


main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);

	if (sem_init(&EmptySem, 0, 0)<0)  { //Empty semaphore를 0개로 초기화
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

	if (pthread_create(&tid1, NULL, (void *)sipc1, (void *)NULL) < 0)  { //Producer thread 생성
		perror("pthread_create");
		exit(1); //실패 시 process 종료
	}

	if (pthread_create(&tid2, NULL, (void *)sipc2, (void *)NULL) < 0)  { //Consumer thread 생성
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

    printf("Main    : End...\n");
	
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
