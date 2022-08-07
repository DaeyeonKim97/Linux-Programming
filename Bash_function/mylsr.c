#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH	256

void JustDoIt(char *path)
{
	DIR		*dp;
	struct dirent	*dep;
	struct stat	statbuf;
	char		fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL)  { //파라미터로 주어진 directory를 열고 실패시 stderr opendir출력 후 종료
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n", path); //파라미터 path 출력

	while (dep = readdir(dp))  { //directory 내의 파일들에 대하여 반복
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue; //현재 디렉토리와 상위 디렉토리에 대해서는 출력하지 않음
		printf("%s\n", dep->d_name); //파일들의 이름을 출력
	}

	rewinddir(dp); //파일 지시자를 시작으로 옮김

	while (dep = readdir(dp))  { //directory 내의 파일들에 대하여 반복
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue; // 현재 디렉토리와 상위 디렉토리에 대해서는 출력하지 않음
		strcpy(fullpath, path); 
		strcat(fullpath, "/"); // 파라미터 path+/
		strcat(fullpath, dep->d_name); // 파라미터 path+/+directory name
		if (lstat(fullpath, &statbuf) < 0)  { // fullpath의 파일 정보를 받아오지 못하면 stderr lstat
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode))  { //directory일 경우
			JustDoIt(fullpath); //directory 내부에 대해서 다시 JustDoIt함수 실행
		}
	}
	
	closedir(dp); //directory close
}

main()
{
	JustDoIt(".");
}
