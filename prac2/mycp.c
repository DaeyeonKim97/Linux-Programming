#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h> 

#define MAX_BUF 1024

main(int argc, char *argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];

	if(argc!=3) {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	} // argument가 정확히 전달되지 않았을 경우 실행경로와 함께 에러메시지 출력
	

    /* 파일 권한 확인 */
	
    if ((fd1 = open(argv[1], O_RDONLY)) < 0){
		perror("open");
		exit(1);
	} // file descripter1 이 Read Only option으로 열리지 않을경우 stderr "open" 출력

	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){ //0644는 소유자 W,R & 다른사람 R
		perror("open");
		exit(1);
	} // file descripter2 가 Write Only, Create New, TRUNC option으로 열리지 않을경우, stderr "open"출력



    /* 파일을 버퍼단위로 read하여 fd2로 write */

	while ((count = read(fd1, buf, MAX_BUF)) > 0){ //EOF까지 open한 fd1을 buf에다가 최대 MAX_BUF만큼 입력받음 
			write(fd2,buf, count);  //fd2에 buf의 count만큼 write
	}  

	close(fd1);
	close(fd2); //fd1, fd2 종료
	
}
