#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 256

main(int argc, char *argv[])
{
	char buf[MAX_BUF];

	if(getcwd(buf, MAX_BUF) ==NULL)
	{
		perror("getcwd");
		exit(1);
	} //현재 작업 디렉토리명을 buffer에 받아오고 NULL일 경우 종료

	printf("%s\n", buf); // buf 출력
}
