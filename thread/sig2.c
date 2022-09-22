#include <stdio.h>
#include <signal.h>

void SigUsrHandler(int signo)
{
	if(signo == SIGUSR1){
		printf("Recieved SIGUSR1 signal\n");
	} //USR1 커스텀변수로 시그널 전달 시
	else if(signo == SIGUSR2){
		printf("Recieved SIGUSR2 signal\n");
	} //USR2 커스텀 변수로 시그널 전달 시
	else{
		printf("Received unknown siganl\n");
		printf("Terminate this process\n");
		exit(0);
	} //그외 시그널 전달 시
}

main()
{
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR){ // SIGUSR 감지 시 SigUsrHandler 호출
		perror("signal"); 
		exit(1);
	} //SIG_ERR 발생하면 perror signal출력 후 종료

	if(signal(SIGUSR2, SigUsrHandler) == SIG_ERR){ // SIGUSR2 감지 시 SigUsrHandler 호출
		perror("signal");
		exit(1);
	} //SIG_ERR 발생하면 perror signal출력 후 종료

	for( ; ; )
		pause(); //프로세스 계속 실행을 위한 코드
}
