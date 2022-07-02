#include <stdio.h>
#include <string.h>
#include "record.h"

main(int argc, char *argv[])
{
	FILE *src, *dst;
	Record rec; //Record Structure

	if (argc !=3){
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	} //입력 형식이 올바르지 않은경우 입력형식 출력 후 종료

	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	} //argv[1]을 read text로 열지 못한 경우 error message 출력 후 종료

	if ((dst = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	} //argv[2]를 write binary로 열지 못한경우 error message 출력 후 종료

	while (fgets(rec.stud,SMAX,src)){ //rec.stud에 한줄을 받아옴
		*strchr(rec.stud, '\n') = '\0'; //개행 문자를 찾고 null로 전환 
		fgets(rec.num, NMAX, src); //rec.num에 뒤 한줄을 받아옴
		*strchr(rec.num, '\n') = '\0'; //개행문자를 찾고 null로 전환
		fgets(rec.dept, DMAX, src);//rec.dept에 뒤 한줄을 받아옴
		*strchr(rec.dept, '\n') = '\0'; //개행 문자를 찾고 null로 전환 

		fwrite(&rec, sizeof(rec), 1, dst); //rec의 내용을 dst에 저장
	}

	fclose(src);
	fclose(dst);
	//file close
}