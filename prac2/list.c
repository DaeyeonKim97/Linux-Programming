#include <stdio.h>

#define MAX_BUF 256

main(int argc, char *argv[])
{
    FILE    *fp;
    char    buf[MAX_BUF];
    int     line;

    if(argc !=2){
        printf("Usage: %s filename\n",argv[0]);
    } //올바른 콘솔 입력 형식 요구

    if(fp = fopen(argv[1], "rt")==NULL){ //파일을 텍스트 읽기로 염
        perror("fopen");
        exit(1);
    }

    line =1;
    while(fgets(buf, MAX_BUF, fp)){  //buf에 파일 내용을 한줄 읽어옴
        printf("%4d: %s", line++, buf);
    }

    fclose(fp);
}