#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	umask(0); //bar을 creat할 때 mask을 하나도 하지 않겠다.
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) 
	{ß
		perror("creat");
		exit(1);
	}
	//사용자 읽기, 사용자 쓰기, 그룹 읽기, 그룹 쓰기, 기타 읽기, 기타 쓰기 권한을 bitwise or 하여 킴
	//실패 시 stderr creat출력 후 종료

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); //file을 생성할 때 그룹쓰기, 기타 읽기, 기타 쓰기권한을 끔
	if(creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0)
	{
		perror("creat");
		exit(1);
	}
	//mask된 것을 제외하고 bitwise or을 하여 킨 다음 creat
	//실패 시 stderr creat출력 후 종료
}
