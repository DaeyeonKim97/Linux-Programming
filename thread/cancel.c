#include <stdio.h>
#include <signal.h>
#include <pthread.h>

pthread_t	ThreadId[2];

void SigIntHandler(int signo) //SIGINT가 발생했을 때 실행시킬 handler
{
	int	i;

	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	//어떤 thread에 의해 SIGINT가 발생하였는지 출력
	printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);
	//다른 thread들의 Id를 출력하고 kill하겠다고 알림

	for (i = 0 ; i < 2 ; i++)  {
		if (pthread_cancel(ThreadId[i]))  {
			perror("pthread_cancel");
			exit(1);
		}
	} //Thread1과 Thread2를 kill
	for (i = 0 ; i < 2 ; i++)  {
		if (pthread_join(ThreadId[i], NULL))  {
			perror("pthread_join");
			exit(1);
		}
	} //Thread1과 Thread2를 join
	printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);
	//Thread들이 종료되었음을 알림
	exit(0);
}

void Thread1(void *dummy) //Thread1이 생성되었을 때
{
	printf("Thread %d created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	} //이 thread는 다른 thread에서 종료시킬 수 있음
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	} //이 thread는 종료시 asynchronous하게 바로 죽을 것임.

	while (1)
		;
}

void Thread2(void *dummy) //Thread2가 생성되었을 때
{
	printf("Thread %d created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	} //이 thread는 다른 thread에서 종료시킬 수 있음
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	} //이 thread는 종료 시 asynchronous하게 바로 죽을 것임.

	while (1)
		;
}

main()
{
	if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// 두 thread를 생성하고 Thread아이디 출력

	signal(SIGINT, SigIntHandler); 
	//SIGINT발생 시 SigIntHandler호출

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();
}
