#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "synclib.h"
#include "shm2.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


int update(long *ptr)
{
	return ((*ptr)++);
}

main()
{
	int	i, counter;
	int	shmid;
	char	*ptr;
	pid_t	pid;	
	

	TELL_WAIT();
	if ((pid = fork()) < 0)  { //현재 process fork => shared memory를 똑같이 가르킴
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  { //parent process

		
		if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
			perror("shmget");
			exit(1);
		}
		if((ptr = shmat(shmid, 0, 0)) == (void*) -1){
			perror("shmat");
			exit(1);
		}

		for (i = 0 ; i < NLOOPS ; i += 2)  { //2씩 증가시키면서 process에서 출력
			if ((counter = update((long *) ptr)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD(); //Child로 context switching
			WAIT_CHILD();
		}
	}
	else  { //child process

		
		if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE))<0){
			perror("shmget");
			exit(1);
		}
		if((ptr = shmat(shmid, 0, 0)) == (void*) -1){
			perror("shmat");
			exit(1);
		}

		for (i = 1 ; i < NLOOPS ; i += 2)  { //2씩 증가시키면서 process에서 출력
			WAIT_PARENT();
			if ((counter = update((long *) ptr)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT(); //Parent로 context switching
		}
	}

	if(shmctl(shmid, IPC_RMID, 0) <0){
		perror("shmctl");
		exit(1);
	}
}
