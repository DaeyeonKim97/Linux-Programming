#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


int update(long *ptr)
{
	return ((*ptr)++);
}

main()
{
	int	fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	if ((fd = open("/dev/zero", O_RDWR)) < 0)  { //dev/zero oepn
		perror("open");
		exit(1);
	} 
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t) -1)  { //fd를 mmap하여 area라는 shared memory를 만듦.
		perror("mmap");
		exit(1);
	}
	close(fd);

	TELL_WAIT();
	if ((pid = fork()) < 0)  { //현재 process fork => shared memory를 똑같이 가르킴
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  { //parent process
		for (i = 0 ; i < NLOOPS ; i += 2)  { //2씩 증가시키면서 process에서 출력
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD(); //Child로 context switching
			WAIT_CHILD();
		}
	}
	else  { //child process
		for (i = 1 ; i < NLOOPS ; i += 2)  { //2씩 증가시키면서 process에서 출력
			WAIT_PARENT();
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT(); //Parent로 context switching
		}
	}
}
