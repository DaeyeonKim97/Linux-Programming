#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	pid_t	pid;
	char	*argv[] = { "mycp", "exec.c", "exec2.c", NULL };
	//argument 설정

	// execl
	if ((pid = fork()) < 0)  { //현재 프로세스 fork실패 시 perror fork출력 후 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  { //자식 프로세스일 경우
		if (execl("/home/lsp04/hw03/mycp",
				"mycp", "exec.c", "exec1.c", NULL) < 0)  { //폴더에 있는 파일들을 execl
			perror("execl"); //실패 시 종료
			exit(1);
		} //execl은 다른 프로그램을 리스트로 받아 실행하고 자신을 종료
	}
	if (waitpid(pid, NULL, 0) < 0)  { //본인 프로세스의 자식 프로세스를 기다림
		perror("waitpid"); //실패 시 perror waitpid출력 후 종료
		exit(1);
	}

	// execv
	if ((pid = fork()) < 0)  { //현재 프로세스 fork실패 시 perror fork출력 후 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		if (execv("/home/lsp04/hw03/mycp", argv) < 0)  { //폴더에 있는 파일들을 execv
			perror("execv"); //실패 시 perror execv 출력 후 종료
			exit(1);
		} //execv는 다른프로그램 리스트의 시작주소를 받아 실행하고 자신을 종료
	}
	if (waitpid(pid, NULL, 0) < 0)  { //본인 프로세스의 자식 프로세스를 기다림
		perror("waitpid");//실패 시 perror waitpid출력 후 종료
		exit(1);
	}

	// execlp
	if ((pid = fork()) < 0)  { //현재 프로세스 fork실패 시 perror fork출력 후 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		if (execlp("env", "env", NULL) < 0)  { //폴더에 있는 파일들을 execlp
			perror("execlp"); //실패 시 perror execlp출력 후 종료
			exit(1);
		} //execlp는 PATH에 등록된 모든 디렉토리에 있는 프로그램을 실행
	} 
	if (waitpid(pid, NULL, 0) < 0)  { //자식 프로세스를 기다림
		perror("waitpid");//실패 시 perror waitpid출력 후 종료
		exit(1);
	}
}
