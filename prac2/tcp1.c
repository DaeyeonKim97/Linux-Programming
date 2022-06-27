#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE    *src, *dst;
    int             ch;

    if(argc !=3){
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        return 1;
    } //입력형식 유효성 검사

    if((src = fopen(argv[1],"rt")) == NULL){
        perror("fopen");
        return 1;
    } //텍스트 읽기 권한으로 source파일을 읽어옴

    if((dst = fopen(argv[2],"wt")) == NULL){
        perror("fopen");
        return 1;
    } //텍스트 쓰기 권한으로 source파일을 읽어옴

    while((ch = fgetc(src)) != EOF){
        fputc(ch,dst);
    } //End of File 까지 char을 읽어와서 dst에 저장.

    fclose(src);
    fclose(dst);

    return 0;
}