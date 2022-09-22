#include <stdio.h>
#include <signal.h>

void SigAlarmHandler(int signo)
{
	return;
}

unsigned int mysleep(unsigned int nsecs)
{
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){ //SIGALRM은 alarm() 함수를 통해 signal을 줄 수 있음
		return nsecs;
	}

	alarm(nsecs); // nsecs뒤에 시그널이 발생

	pause(); //시그널 수신 때 까지 대기 (알람이 아닌 어떤 시그널이 와도 pause가 풀림)

	return alarm(0); // 5초를 기다리지 못했을경우 0초로 초기화.
}

main()
{
	printf("Wait for 5 secons...\n");

	mysleep(5);
}
