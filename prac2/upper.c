#include <stdio.h>


main(int argc, char *argv[])
{
	FILE	*src;
	//FILE	*dst;  //=> 파일로 출력할때 사용
	int	ch;

	if (argc !=2){
		fprintf(stderr, "Usage: %s source \n", argv[0]);
	} //입력 형태가 틀렸을 경우 입력 형태를 출력하고 종료


	if ((src = fopen(argv[1], "rt")) == NULL){
		perror("fopen");
		return 1;
	} //argv[1]을 read text로 출력하지 못했을 경우 에러메시지를 출력하고 종료

	//if ((dst = fopen(strcat(argv[1],".upper"), "wt")) == NULL){
	//	perror("fopen");
	//	exit(1);
	//} //=> 파일로 출력할때 사용

	while ((ch = fgetc(src)) != EOF){ //더이상 받아올 문자가 없을 때 까지 반복
		if(ch>='a' && ch<='z'){
			ch = ch-32;
		} //문자가 ASKII기준 소문자일 경우 32를 빼서 ASKII대문자로 변환
		putchar(ch);
		//fputc(ch, dst); //=> 파일로 출력할때 사용
	}

	fclose(src);
	//fclose(dst); //=> 파일로 출력할 때 사용
}
