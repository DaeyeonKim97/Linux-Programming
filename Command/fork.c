#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int	Var = 6;
char	Buf[] = "a write to stdout\n";

main()
{
	int	var;
	pid_t	pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1); //stdout file descripter를 통해 stdout에 Buf의 내용을 write.
	printf("Before fork\n");

	if ((pid = fork()) < 0){ //현재 프로세스를 fork하는데 실패했을 경우
		perror("fork"); //perror fork 출력 후 종료
		exit(1);
	}
	else if(pid == 0){ //Child process에서는 global variable의 값을 2추가
		Var++;
		Var++;	//Var += 2
	}
	else { //parent process에서 아래 출력을 진행
		sleep(2); //2ms 만큼 딜레이
	}
	
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var); //global variable, local variable의 값을 출력

}
