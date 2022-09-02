#include <stdio.h>

#define	MAX_CMD	256

void DoCmd(char *cmd)
{
	printf("Doing %s", cmd); //함수의 시작 알림
	sleep(1); //대기
	printf("Done\n");//함수의 종료 알림
}

main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); //stdin으로 명령어를 입력받음
		if (cmd[0] == 'q')
			break;

		if ((pid = fork()) < 0)  { // 프로세스 fork후 실패시 perror fork출력 후 종료
			perror("fork");
			exit(1);
		}
		else if (pid == 0)  { // 자식프로세스에서 cmd를 실행
			DoCmd(cmd);
			exit(0);
		}
#if 0
		else  {
			wait(NULL);
		}
#endif
	}
}
