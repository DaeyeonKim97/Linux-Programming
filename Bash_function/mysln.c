#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage : %s source destination\n", argv[0]);
		exit(1);
	} //입력형식에 맞지 않을경우 입력형식 출력 후 종료
	if(symlink(argv[1], argv[2])<0)
	{
		perror("link");
		exit(1);
	} //argv[1]을 symbolic link하여 argv[2]를 만들고 실패시 stderr link출력 후 종료
}
