#include <stdio.h>
#include <stdlib.h>
#define MAX_BUF 1024

int main(int argc, char* argv[]){
    //Definite field
    FILE    *src1, *src2, *dst;
    char    buf[MAX_BUF];
    int     count;

    //Check input validation
    if(argc !=4){
        fprintf(stderr,"Usage: %s source1, source2, dest\n",argv[0]);
        return 1;
    }

    //Open file by Binary
    if((src1 = fopen(argv[1],"rb"))==NULL){
        perror("fopen");
        return 1;
    }
    if((src2 = fopen(argv[2],"rb"))==NULL){
        perror("fopen");
        return 1;
    }
    if((dst = fopen(argv[3],"wb"))==NULL){
        perror("fopen");
        return 1;
    }

    // Main program: Merge src1, src2 to dst
    while((count = fread(buf,1,MAX_BUF,src1))>0){
        fwrite(buf,1,count,dst);
    }
    while((count = fread(buf,1,MAX_BUF,src2))>0){
        fwrite(buf,1,count,dst);
    }

    //File close
    fclose(src1);
    fclose(src2);
    fclose(dst);

}