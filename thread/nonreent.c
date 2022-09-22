#include <stdio.h>
#include <signal.h>
#include <pwd.h>

void MyAlarmHandler(int signo)
{
	struct passwd	*rootptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1); //1초뒤에 Alarm을 발생시켜서 MyAlarmHandler를 재 호출

	printf("in signal handler\n");

	if((rootptr = getpwnam("root")) == NULL){ //1초마다 root에 대한 정보를 읽어 옴
		perror("getpwnam");
		exit(1);
	}
	//getpwname은 함수 안에 static struct passwd pw를 선언하고 받아 옴
	//=> 구조체의 내용이 root로 바뀜

	return;
}

main()
{
	struct passwd	*ptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1); //1초마다 signal을 발생시켜서 MyAlarmHandler를 호출

	for( ; ; ){
		if((ptr = getpwnam("lsp04")) == NULL){ //loop를 돌며 lsp04의 password를 읽어 옴.
			perror("getpwnam")	
			exit(1);
		}
	}

	if(strcmp(ptr->pw_name, "lsp04") != 0){ //ptr의 username은 당연히 lsp04가 되어야 함.
		printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
		exit(0);
		//하지만 alarm handler에서 static변수가 변해서 root를 보여줌
		//non re enterent function임.
	}
