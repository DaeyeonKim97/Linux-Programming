#include <stdio.h>

main(int argc, char *argv[])
{
	if(argc != 3){
		fprintf(stderr, "Usage: %s old new \n", argv[0]);
	} // 입력 형식에 맞지 않을 경우 올바른 입력 형식 출력 후 종료

	if(rename(argv[1], argv[2]) <0){
		perror("rename");
		exit(1);
	} // argv[1]을 argv[2]로 이름을 바꾸고 실패시 stderr rename출력 후 종료
}
