#include <stdio.h>
#include <signal.h>

static unsigned int AlarmSecs; //전역변수(이파일만 사용할 수 있도록 static)

void SigAlarmHandler(int signo)
{
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	} //알람을 받는 signal을 하나 더 추가함.(재귀) => 무한반복

	alarm(AlarmSecs);

	/*Do something*/
	printf(".");
	fflush(stdout); // \n이 안나와도 buffer cache를 비워라.

	return;
}

int SetPeriodicAlarm(unsigned int nsecs)
{
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return -1;
	} //알람이 발생하면 SigAlarmHandler를 호출

	AlarmSecs = nsecs; //전역변수 AlarmSecs를 nsecs로 설정

	alarm(nsecs); //nsecs마다 알람을 울림.

	return 0;
}

main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1);

	for( ; ; )
		pause();
}
