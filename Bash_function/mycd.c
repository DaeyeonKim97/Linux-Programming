#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc !=2)
	{
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	} //입력형식에 맞지 않을 경우 입력형식을 출력하고 종료
	if(chdir(argv[1])<0)
	{
		perror("chdir");
		exit(1);
	} //argv[1]으로 directory를 바꾸고 실패시 종료
}
