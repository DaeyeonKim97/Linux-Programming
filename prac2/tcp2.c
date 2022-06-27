#include <stdio.h>
#define MAX_BUF 256

int main(int argc, char *argv[])
{
    FILE    *src, *dst;
    char    sentBuf[MAX_BUF];

    //입력 형식 유효성 검사
    if(argc != 3){
        fprintf(stderr, "Usage: %s source destination\n",argv[0]);
    }

    //source file open
    if((src = fopen(argv[1],"rt")) == NULL){
        perror("fopen");
        return 1;
    }

    //destination file open
    if((dst = fopen(argv[2],"wt")) == NULL){
        perror("fopen");
        return 1;
    }

    //main prg => line단위로 파일내용을 복사해옴
    while((fgets(sentBuf, MAX_BUF, src)) != NULL){
        fputs(sentBuf,dst);
    }

    //file close
    fclose(src);
    fclose(dst);
}