#include <pthread.h>

typedef struct  {
	int		sval; //세마포어의 개수
	pthread_mutex_t	mutex; //critical section을 보호하기 위한 mutex
	pthread_cond_t	cond; //condition variable
}
	sem_t;

int		sem_init(sem_t *sem, int pshared, int value);
int		sem_wait(sem_t *sem);
int		sem_trywait(sem_t *sem);
int		sem_post(sem_t *sem);
int		sem_getvalue(sem_t *sem, int *sval);
int		sem_destroy(sem_t *sem);
