#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

#define	LEFT(i)		((i+NUM_MEN-1)%NUM_MEN) //i번째 철학자의 왼쪽 철학자의 번호
#define	RIGHT(i)	((i+1)%NUM_MEN) //i번째 철학자의 오른쪽 철학자의 번호

enum  { THINKING, EATING, HUNGRY }; //철학자의 상태를 THINKING, EATING, HUNGRY 세 개의 상태로 구분

sem_t	Philosopher[NUM_MEN];
sem_t	Mutex;
int	State[NUM_MEN];

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

void Thinking(int id) //Thinking 시간. thread를 sleep
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void Eating(int id) //Eating 시간. thread를 sleep
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void Test(int id) //내 왼쪽 오른쪽의 철학자가 먹고 있는지 확인하고 안먹고 있을경우 eating
{
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING && State[RIGHT(id)] != EATING)  { 
		//내가 HUNGRY이고 왼쪽과 오른쪽이 먹고있지 않을 경우
		State[id] = EATING; //철학자의 상태를 먹고있다고 설정

		if (sem_post(&Philosopher[id]) < 0)  { //Philosopher id번째의 semaphore에 signal
			perror("sem_post");
			pthread_exit(NULL);//실패 시 thread 종료
		}
	}
}

void PickUp(int id) //젓가락을 집는 것. Test과정을 거침
{
	if (sem_wait(&Mutex) < 0)  { //Mutex semaphore를 대기
		perror("sem_wait");
		pthread_exit(NULL); //실패 시 thread 종료
	}

	State[id] = HUNGRY; //철학자의 상태를 HUNGRY로 설정

	Test(id); //양쪽 철학자의 상태를 확인하고 먹고있는상태가 아닐경우 Eating

	if (sem_post(&Mutex) < 0)  { //Mutex semaphore에 signal
		perror("sem_post");
		pthread_exit(NULL); //실패 시 thread 종료
	}

	if (sem_wait(&Philosopher[id]) < 0)  { //Philosopher semaphore에 signal
		perror("sem_wait");
		pthread_exit(NULL); //실패 시 thread 종료
	}
}

void PutDown(int id) //젓가락을 내려놓음, 양쪽 철학자 Test
{
	if (sem_wait(&Mutex) < 0)  { //Mutex semaphre를 wait
		perror("sem_wait");
		pthread_exit(NULL); //실패 시 thread 종료
	}

	State[id] = THINKING; //해당 철학자의 상태를 THINKING으로 바꿈

	Test(LEFT(id)); //LEFT철학자에 대하여 Test
	Test(RIGHT(id)); //Right철학자에 대하여 Test

	if (sem_post(&Mutex) < 0)  { //Mutex semaphore에 signal
		perror("sem_post");
		pthread_exit(NULL);//실패 시 thread 종료
	}
}

void DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId;

	for (i = 0 ; i < NLOOPS ; i++)  { //NLOOPS번 반복

		Thinking(id); //id 번째 철학자 Thinking

		PickUp(id); //id 번째 철학자 Pickup

		Eating(id); //id 번째 철학자 Eating

		PutDown(id); //id 번째 철학자 Putdown
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i); //id번째 철학자가 몇번 진행했는지

	pthread_exit(NULL); //Thread 종료
}

main()
{
	pthread_t	tid[NUM_MEN];
	int		i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자 수만큼 반복
		if (sem_init(&Philosopher[i], 0, 0) < 0)  { //Philsopher의 semaphore를 초기화
			perror("sem_init");
			exit(1); //실패 시 process 종료
		}
		id[i] = i; 
	}
	if (sem_init(&Mutex, 0, 1) < 0)  { //Mutex semaphore를 초기화
		perror("sem_init");
		exit(1); //실패 시 process 종료
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자들의 thread를 생성하여 Dining Pilosopher실행
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);//실패 시 종료
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자들의 thread 종료를 기다림
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { //철학자들의 semaphore를 삭제
		if (sem_destroy(&Philosopher[i]) < 0)  {
			perror("sem_destroy");
		}
	}
}
