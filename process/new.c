#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


int compare_time(struct stat statbuf1, struct stat statbuf2) //시간을 비교하는 함수
{
	char time1[32];
	char time2[32];

	strcpy(time1, ctime(&statbuf1.st_mtime)); //time1에 statbuf1의 시간을 string형태로 받아옴
	strcpy(time2, ctime(&statbuf2.st_mtime)); //time2에 statbuf2의 시간을 string형태로 받아옴

	return strcmp(time1, time2); //time1이 크면 양수, time2가 크면 음수, 같으면 0을 반환함
}

int main()
{
	DIR	*dp;
	struct 	dirent	*dep;
	struct	stat	statbuf;
	struct	stat	statbuf_new;
	char*	name;

	if ((dp = opendir("."))==NULL) //현재 디렉토리를 엶
	{
		perror("opendir");
		exit(0);
	}
	
	int first = 1; // 비교의 첫 시작을 알리기 위한 변수

	while(dep = readdir(dp)) //디렉토리 내의 모든 파일들에 대하여 반복
	{

		//if(strcmp(dep->d_name ,".")==0 || strcmp(dep->d_name, "..")==0)
		//	continue;
		//같은 파일이기 때문에 .과 ..도 검사해야 된다고 하심.
		//마지막 수정시간 기준

		if(stat(dep->d_name, &statbuf) < 0) //file의 stat을 받아옴 
		{
			perror("stat");
			exit(1);
		}
		
		if(first == 1){ //첫 비교의 경우 가장 최근의 파일을 현재 파일로 설정하고 continue
			statbuf_new = statbuf;
			name = dep->d_name;
			first = 0;

			continue;
		}

		if(compare_time(statbuf, statbuf_new) >= 0){ //더 최근의 파일이 생겼을 경우 신규 파일에 대한 정보를 교체
			statbuf_new = statbuf;
                        name = dep->d_name;

		}
		else{
			
		}

	}

	printf("%s\n", name); //가장 최신의 파일을 출력


}
