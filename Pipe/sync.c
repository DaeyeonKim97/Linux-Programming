#include <stdio.h>
#include "synclib.h"

#define NLOOPS 5

main()
{
	int	i;
	pid_t	pid;

	TELL_WAIT();
	if((pid = fork()) < 0){ //현재 process fork 실패시 perror fork 후 종료
		perror("fork");
		exit(1);
	}

	else if (pid > 0){ //Parent process일 경우
		for(i = 0; i <NLOOPS; i++){ //NLOOPS번동안 
			TELL_CHILD(); //Pfd1에 "p"를 보냄
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD(); //Pfd2에서 "c"를 기다림
		}
	}
	else { //Child process의 경우
		for(i=0; i<NLOOPS; i++){
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT(); //Pfd1에서 "p"를 기다림
			TELL_PARENT(); //Pfd2에 "c"를 보냄
			printf("Child: Tell to parent\n");
		}
	}
}
