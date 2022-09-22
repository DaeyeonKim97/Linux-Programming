#include <stdio.h>
#include <signal.h>

void SigIntHandler(int signo)
{
	printf("Recived a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
} //SIGINT signal을 받았다는 출력과 함께 프로세수를 종료하는 함수

main()
{
	signal(SIGINT, SigIntHandler); //process 종료 시(^C) SigIntHandler 호출

	printf("Press ^C to quit\n");
	
	for( ; ; )
		pause();
	//process가 계속 동작하도록 for문에 pause를 걸어줌
}
