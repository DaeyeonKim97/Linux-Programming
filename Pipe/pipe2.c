#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 128

main()
{
	int	n, fd[2], fd2[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	if(pipe(fd) < 0){ //pipe를 통해 두개의 파일 디스크립터를 생성하고 하나의 파이프를 프로세스들이 공유
		//fd[0] : 파이프 출구 (읽는 쪽)
		//fd[1] : 파이프 입구 (쓰는 쪽)
		perror("pipe");
		exit(1);
	}

	if(pipe(fd2) <0){
		perror("pipe");
		exit(1);
	}

	if((pid = fork()) < 0) // 현재 process를 fork, fork실패 시 error message 출력 후 종료, pipe는 fork되지 않음
	{
		perror("fork");
		exit(1);
	}

	else if (pid == 0) //Child process
	{
		printf("Child : Wait for parent to send data\n");
		if((n = read(fd[0], buf, MAX_BUF)) < 0){ //파일의 내용을 read하고 실패시 perror read
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout); //출력 버퍼 안에 존재하는 데이터를 비우는 즉시 출력
		write(STDOUT_FILENO, buf, n); //buf의 내용을 화면에 출력

		printf("Child: Send data to parent\n");
		strcpy(buf, "Child to Parent\n");
		write(fd2[1], buf, strlen(buf)+1);

	}
	else{
		strcpy(buf, "Parent to Child\n"); // buf에 string을 씀
		printf("Parent: Send data to child\n"); 
		write(fd[1], buf, strlen(buf)+1); // 파이프에 buf의 내용을 씀

		printf("Parent : Wait for child to send data\n");
		if((n = read(fd2[0], buf, MAX_BUF)) < 0){ //파일의 내용을 read하고 실패시 perror read
			perror("read");
			exit(1);
		}
		printf("Parent : Received data from child: ");
		fflush(stdout); //출력 버퍼 안에 존재하는 데이터를 비우는 즉시 출력
		write(STDOUT_FILENO, buf, n); //buf의 내용을 화면에 출력
	}

	exit(0);
}

