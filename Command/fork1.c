#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	int	pid;
	if((pid=fork()) < 0){ //현재 프로세스를 복제하지 못했을 경우 perror fork 출력 후 종료
		perror("fork");
		exit(1);
	}
	
	else if (pid==0){ //지금 실행중인 프로세스가 child process의 경우
		/*child*/
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
		//getpid()를 통해 calling process의 processID 출럭
		//gitppid()를 통해 calling process의 processID 출력
	}

	else { //parent process의 경우 pid에 child processID를 받아옴
		/*parent*/
		printf("I am %d. My child is %d. \n", getpid(), pid);
		//gitpid()를 통해 calling process의 processID 출력
	}
}
