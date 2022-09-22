#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void SigChldHandler(int signo) //SIGCHLD가 발생했을 때의 Handler
{
	printf("Received a SIGCHLD signal\n");
	printf("Child process terminated\n");
	printf("Terminate Parent process\n");

	exit(0);
}

main()
{
	pid_t 	pid;
	int	status;

	signal(SIGCHLD, SigChldHandler); //Child process가 종료를 signal을 이용하여 asynchronous하게 처리


	if ((pid = fork())<0){
		perror("fork");
		exit(1);
	} //fork 실패 시 perror fork출력 후 종료

	else if (pid == 0){
		//자식 프로세스의 경우 자식프로세스임을 출력
		printf("I'm a child\n");
		sleep(2);
	}
	else {
		while(1) ;
	}
}
