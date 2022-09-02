#include <stdio.h>
#include <pthread.h>

#define	MAX_CMD	256

void DoCmd(char *cmd) //thread를 통해 cmd를 실행
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");

	pthread_exit(NULL);
}

main()
{
	char		cmd[MAX_CMD];
	pthread_t	tid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); //stdin을 통해 cmd를 받아옴
		if (cmd[0] == 'q')
			break;

		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0)  { //thread를 만들어 DoCmd호출
			perror("pthread_create");//실패 시 perror pthread_create
			exit(1);
		}

#if 0
		pthread_join(tid, NULL); //thread join
#endif
	}
}
