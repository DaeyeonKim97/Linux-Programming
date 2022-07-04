#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { FALSE, TRUE };

void conv(char *fname) //각 줄마다 first tab을 ' '4개로 바꿔주는 함수
{
	FILE	*src, *dst;
	char	fdest[40];
	int		ch, first;

	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	} //파라미터로 주어진 파일을 read text로 열고 열지 못할경우 에러메시지 출력 후 종료

	strcpy(fdest, fname); //fname을 fdest에 복사
	strcat(fdest, ".t"); //fdest를 filename.t형태로 변환
	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");
		return;
	} //filename.t를 write text로 열지 못할 경우 에러메시지 출력 후 종료

	first = TRUE;
	while ((ch = fgetc(src)) != EOF)  { //EOF까지 반복
		if (first && ch == '\t')  { //first가 TRUE이고 입력 character가 tab일때 
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst); //tab대신 ' ' 4개를 dst에 put
		}
		else  { //그 외의 경우
			fputc(ch, dst); //입력 character를 그대로 넣음
			if (first)
				first = FALSE; //first를 FALSE로
			if (ch == '\n')
				first = TRUE; //줄 바꿈이 나오면 초기화
		}
	}

	fclose(src);
	fclose(dst);
	//file close
}

main(int argc, char *argv[])
{
	while (--argc)  {
		conv(argv[argc]);
	} //모든 argv들에 대해서 conv를 실행.
}
