#include <stdio.h>
#include <stdlib.h>

void myexit1()
{
	printf("first exit handler\n");
	//first exit handler를 출력하는 함수
}

void myexit2()
{
	printf("second exit handler\n");
	//second exit handler를 출력하는 함수
}

main()
{
	if(atexit(myexit2) != 0){ // myexit2()의 register exit을 실패 했을 경우
		perror("atexit"); // perror atexit을 출력하고 종료
		exit(1);
	}

	if(atexit(myexit1) != 0) // myexit1()의 register exit을 실패 했을 경우
	{
		perror("atexit"); // perror atexit을 출력하고 종료
		exit(1);
	}

	if(atexit(myexit1) != 0){ // myexit1()의 두 번째 register exit을 실패 했을 경우
		perror("atexit"); // perror atexit을 출력하고 종료
		exit(1);
	}

	//결과적으로 myexit2()가 한 번, myexit1()이 두 번 실행.

	printf("main is done\n");
	//main의 종결 상태를 출력
	
	// main의 종료 이후 두 함수의 register exit이 실행됨
}
