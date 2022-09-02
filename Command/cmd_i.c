#include <stdio.h>

#define	MAX_CMD	256

void DoCmd(char *cmd)
{
	printf("Doing %s", cmd); // 함수의 시작을 알림
	sleep(1); // 1ms 대기
	printf("Done\n"); // 함수의 종료를 알림
}

main()
{
	char	cmd[MAX_CMD];

	while (1)  {
		printf("CMD> "); 
		fgets(cmd, MAX_CMD, stdin); //stdin으로 명령어를 입력받음	
		if (cmd[0] == 'q') //q가 나올경우 명령 종료
			break;

		DoCmd(cmd); // 함수 호출
	}
}
