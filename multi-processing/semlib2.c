#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


int sem_init(sem_t *sem, int pshared, int value) //세마포어를 하나 만들고 value로 초기화
	//pshared : POSIX세마포어에서 process간 sharing을 하는가?(지원 안하므로 항상 0)
{
	if (pshared)  {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0) //mutex를 하나 만듦
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0) //condition variable을 하나 만듦
		return -1;

	sem->sval = value; //세마포어의 값을 parameter value로 초기화

	return 0;
}

int sem_wait(sem_t *sem) //세마포어를 waiting (-1)
{
	if (pthread_mutex_lock(&sem->mutex) < 0) //critical section이므로 lock
		return -1;

	if (sem->sval == 0)  { //세마포어의 값이 0이면 
		while (sem->sval == 0)  {
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  { //condition variable을 기다림
				if (pthread_mutex_unlock(&sem->mutex) < 0) //락을 풀고 -1로 return
					return -1;
				return -1;
			}
		}
		sem->sval--; //waiting이 끝나고 -1
	}
	else  {
		sem->sval--; //0이 아니면 그냥 -1
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) //빠져 나올 때 unlock
		return -1;

	return 0;
}

int sem_trywait(sem_t *sem) //wait하고 똑같은데 condition variable을 기다리느냐 기다리지 않느냐
{
	if (pthread_mutex_lock(&sem->mutex) < 0) //critical section이므로 lock
		return -1;

	if (sem->sval == 0)  { //세마포어의 값이 0이면
		if (pthread_mutex_unlock(&sem->mutex) < 0) //락을 풀고 -1 return
			return -1;
		return -1;
	}
	else  {
		sem->sval--; //0이 아닐경우 똑같이 동
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) //mutex unlock
		return -1;

	return 0;
}

int sem_post(sem_t *sem) //세마포어의 값 증가
{
	if (pthread_mutex_lock(&sem->mutex) < 0) //critical section이므로 lock
		return -1;

	if (sem->sval == 0)  { //세마포어 값이 0인경우
		if (pthread_cond_signal(&sem->cond) < 0)  { //cond signal로 깨워줌
			if (pthread_mutex_unlock(&sem->mutex) < 0) //언락 후 -1 return 종료
				return -1;
			return -1;
		}
	}

	sem->sval++; //세마포어의 값 +1

	if (pthread_mutex_unlock(&sem->mutex) < 0) //critical section unlock후 whdfy
		return -1;

	return 0;
}

int sem_getvalue(sem_t *sem, int *sval) //세마포어의 값을 받아옴
{
	*sval = sem->sval;

	return 0;
}

int sem_destroy(sem_t *sem) //세마포어를 삭제
{
	if (pthread_mutex_destroy(&sem->mutex) < 0) //mutex삭제
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0) //condition variable 삭제
		return -1;

	return 0;
}

