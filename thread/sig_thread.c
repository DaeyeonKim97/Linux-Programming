#include <stdio.h>
#include <signal.h>
#include <pthread.h>

/*
#define	THREAD_MAIN
#define	THREAD_1
*/
#define	THREAD_2
//thread2에서 처리하도록 define하였을 때 결과가 어떻게 되나?



void SigIntHandler(int signo) //SIGINT가 발생시
{
	printf("Received a SIGINT signal by thread %d\n", pthread_self()); //현재 thread의 ID를 출력
	printf("Terminate this process\n"); //현재 process를 종료시킴

	exit(0);
}

void Thread1(void *dummy) //Thread1 에서 실행시킬 function
{
#ifdef	THREAD_1
	signal(SIGINT, SigIntHandler); //SIGINT가 발생하면 SigIntHandler를 동작시킴
#endif

	while (1) //thread를 계속 진행
		;
}

void Thread2(void *dummy) //Thread2 에서 실행시킬 function
{
#ifdef	THREAD_2
	signal(SIGINT, SigIntHandler); //SIGINT가 발생하면 SigIntHandler를 동작시킴
#endif

	while (1) //thread를 계속 진행
		;
}

main()
{

	pthread_t	tid1, tid2; //unsigned int

	if (pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	} // thread1을 만듦
	if (pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	} // thread2를 만듦
	printf("Create two threads: tid1=%lu, tid2=%lu\n", tid1, tid2);
	printf("Main thread: tid=%d\n", pthread_self());

#ifdef	THREAD_MAIN
	signal(SIGINT, SigIntHandler); //signal handler를 main thread에서 실행함
#endif

	printf("Press ^C to quit\n"); //SIGINT가 발생했을 때 어떤 thread에서 발생하는지 확인
					// thread2를 define하였지만 main thread에서 signal을 처리
	for ( ; ; )
		pause();
}
