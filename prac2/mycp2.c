#include <stdio.h>
#define MAX_BUF 1024

int main(int argc, char* argv[]){
    //Field
    FILE    *src, *dst;
    char    buf[MAX_BUF];
    int     count;

    //argument validation
    if(argc != 3){
        fprintf(stderr,"Usage: %s source destination\n",argv[0]);
        return 1;
    }

    //File open (read/write binary)
    if((src = fopen(argv[1],"rb"))==NULL){
        perror("fopen");
    }

    if((dst = fopen(argv[2],"wb"))==NULL){
        perror("fopen");
    }

    //Main Program (binary copy using fread)
    while((count = fread(buf, 1, MAX_BUF, src))>0){ //1개의 바이트단위로 1024개를 읽어옴
        fwrite(buf, 1, count, dst);
    }

    //File close
    fclose(src);
    fclose(dst);
}