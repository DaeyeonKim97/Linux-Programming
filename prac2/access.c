#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "record.h"

void print_record(Record *rp) //record structure를 출력하는 함수
{
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

int main(int argc, char *argv[])
{
	FILE	*fp;
	Record	rec;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		return 1;
	} //입력 형식에 맞지 않을 경우 입력형식을 출력하고 종료

	if ((fp = fopen(argv[1], "r+b")) == NULL)  {
		perror("fopen");
		return 1;
	} //argv[1]을 read binary로 열지 못할경우 에러메시지를 출력하고 종료

	printf("-----Ordered Record List-----\n");

	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	} //내용을 순서대로 출력. record단위로 받아와서 이를 콘솔에 출력
	rewind(fp); //fp 버퍼에 있는 모든 값을 제거
	getchar(); // 버퍼에 남아있는 문자를 초기화

	printf("-----Shuffled Record List-----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec); // 시작에서 커서를 record size의 두배만큼 이항하여 출력

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec); // 시작에서 커서를 record size의 다섯배 만큼 이동하여 출력

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp); 
	print_record(&rec); // 시작에서 커서를 record size만큼 이동하여 출력

	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec); // 시작에서 커서를 record size만큼 이동하여 출력

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec); // 시작에서 커서를 이동하지 않고 출력

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec); //시작에서 커서를 record size의 4배만큼 이동하며 출력
	getchar();//버퍼 초기화

	printf("-----Updated Record List-----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Movie");
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);
	// 시작에서 record size의 2배만큼 이동하여 그 record의 dept값을 Movie로 치환

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);
	// 시작에서 record size의 5배 만큼 이동하여 그 record의 dept값을 Baseball로 치환

	rewind(fp); //fp버퍼에 있는 모든 값을 제거
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	} //처음부터 끝까지 fp의 값을 출력

	fclose(fp);
	//file close
}
