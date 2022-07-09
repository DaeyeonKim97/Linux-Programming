#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	struct stat		statbuf;

	if(stat("bar", &statbuf) < 0) //bar의 상태정보를 읽어오지 못했을 경우 stderr stat출력후 종료
	{
		perror("stat");
		exit(1);
	}
	if(chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID)<0) 
	{
		perror("chmod");
		exit(1);
	} // 현재의 mode에서 그룹 쓰기 권한만 끄고, 실행 시 유효 사용자 ID를 설정함. 실패시 stderr chmod출력 후 종료 

	if(chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
	{
		perror("chmod");
		exit(1);
	} // foo의 사용자 읽기, 사용자 쓰기, 그룹 읽기, 기타 읽기 권한을 킴. 실패시 stderr chmod 출력 후 종료
}
