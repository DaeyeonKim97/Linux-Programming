#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if(argc !=2){
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}//입력 형식에 맞지 않을 경우 입력형식 출력 후 종료
	if(rmdir(argv[1])<0){
		perror("rmdir");
		exit(1);
	}//argv[1]의 directory를 삭제하고 실패 시 stderr rmdir 출력 후 종료
}
