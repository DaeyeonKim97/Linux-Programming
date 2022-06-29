#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 1024

//file size checking function
long fileSize(FILE *fp){
    long cur,size;
    cur = ftell(fp); // 파일 포인터의 현재 위치 저장
    fseek(fp, 0L, SEEK_END); // 파일 끝으로 부터 0L 만큼 이동
    size = ftell(fp); // 파일 끝의 포인터 위치가 곧 size
    fseek(fp, cur, SEEK_SET); // 원래 위치로 이동
    return(size);
}

int main(int argc, char *argv[]){
    //field 선언
    FILE    *src, *dst1, *dst2;
    char    buf[MAX_BUF];
    int     count,size;

    //Checking input validation
    if(argc !=4){
        fprintf(stderr, "Usage : %s source dest1 dest2\n",argv[0]);
        exit(1);
    }

    //Open file by Binary
    if((src = fopen(argv[1],"rb"))==NULL){
        perror("fopen");
        exit(1);
    }
    if ((dst1 = fopen(argv[2], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}
    if ((dst2 = fopen(argv[3], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}

    //Main Function: Split src file to dst1, dst2
    size = filesize(src) / 2;
    while(size>0){
        count = size > MAX_BUF ? MAX_BUF : size; // count는 MAX_BUF와 size중에서 작은 값
        fread(buf, 1, count, src); // 1byte단위로 src에서 buf에 count 개수만큼 읽어옴
        fwrite(buf, 1, count, dst1); // 1byte 단위로 buf 내용을 dst1에 쓰기
        size -= count; // 처리한 양만큼 size에서 빼줌
    }

    while((count = fread(buf,1,MAX_BUF,src)>0)){
        fwrite(buf, 1, count, dst2);
    } //src에 남은 내용들을 읽어와서 dst2에 write


    //File Close
    fclose(src);
    fclose(dst1);
    fclose(dst2);
}