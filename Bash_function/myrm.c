#include <stdio.h>

main(int argc, char *argv[])
{
	int	i;

	if(argc == 1){
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	} //입력 형식에 맞지 않을 경우 입력형식 출력 후 종료

	for(i=1; i<argc; i++)
	{
		if(remove(argv[i])<0)
		{
			perror("remove");
			exit(1);	
		}
	} //argv로 주어진 파일들을 remove하고 실패시 stderr remove출력 후 종료
}
