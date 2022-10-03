#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_MEN	5
#define	NLOOPS	5

sem_t	ChopStick[NUM_MEN];

void ThreadUsleep(int usecs) //usecs 동안 thread를 sleep하는 함수
{
	pthread_cond_t	cond;
	pthread_mutex_t	mutex;
	struct timespec	ts;
	struct timeval	tv;

	if(pthread_cond_init(&cond,NULL) <0){
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}

	if(pthread_mutex_init(&mutex, NULL)<0){
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec=tv.tv_sec + usecs/1000000;
	ts.tv_nsecs = (tv.tv_sec + (usecs/1000000)) * 1000;

	if(ts.tv_nsec > 1000000000){
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if(pthread_mutex_lock(&mutex) <0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	if(pthread_cond_timedwait(&cond, &mutex, &ts)<0){
		perror("pthread_cond_timewait");
		pthread_exit(NULL);
	}

	if(pthread_cond_destroy(&cond) <0){
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}

	if(pthread_mutex_destroy(&mutex) <0){
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void Thinking (int id) //id의 철학자가 생각하는 시간. Thread를 랜덤시간동안 sleep
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void Eating(int id) //id의 철학자가 먹는시간. Thread를 랜덤시간동안 sleep
{
	printf("Philosopher%d: Eating.....\n",id);
	ThreadUsleep((rand()%100)*10000);
}

void DiningPhilosopher(int *pId)
{
	int	i;
	int	id = *pId; //철학자의 id

	for(i=0; i<NLOOPS; i++) //NLOOPS번 반복
	{
		/*Thinking*/
		Thinking(id);

		if(sem_wait(&ChopStick[id])<0){ //ChopStick 세마포어를 기다림
			perror("sem_wait");
			pthread_exit(NULL); //실패 시 thread종료
		}

		if(sem_wait(&ChopStick[(id+1) % NUM_MEN]) <0){ //다음사람의의 ChopStick 세마포어를 기다림
			perror("sem_wait");
			pthread_exit(NULL); //실패 시 thread 종료
		}

		/*Eating*/
		Eating(id);

		if (sem_post(&ChopStick[id]) < 0)  { //먹고나서 자신의 Chopstick 세마포어에 signal
			perror("sem_post");
			pthread_exit(NULL);//실패 시 thread 종료
		}
		if (sem_post(&ChopStick[(id+1) % NUM_MEN]) < 0)  { //자신의 옆사람 Chopstick 세마포어에 signal
			perror("sem_post");
			pthread_exit(NULL);//실패 시 thread 종료
		}
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i); //몇번 완료하였는지?

	pthread_exit(NULL); //Thread 종료
}

main()
{
	pthread_t	tid[NUM_MEN]; //철학자 만큼 thread 생성
	int		i, id[NUM_MEN];

	srand(0x8888); //random seed

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자들의 Chopstick semaphore를 1개로 초기화
		if (sem_init(&ChopStick[i], 0, 1) < 0)  {
			perror("sem_init");
			exit(1);//실패 시 프로세스 종료
		}
		id[i] = i;
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자들의 Thread 생성
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher,
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1); //실패 시 프로세스 종료
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자들의 Threads 종료를 기다림
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1); //실패 시 프로세스 종료
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { //사용했던 semaphores를 destroy
		if (sem_destroy(&ChopStick[i]) < 0)  {
			perror("sem_destroy");
		}
	}
}
