#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int		fd;
	char	ch='\0';

	if((fd = creat("file.hole",0400))<0)
	{
		perror ("creat");
		exit (1);	
	} //file.hole를 사용자에게 읽기권한을 부여하여 만들고 실패할경우 stderr creat출력	

	if(lseek(fd, 1023, SEEK_SET) < 0) { //fd의 시작부터 1023만큼 이동
		perror("lseek");
		exit(1);
	} //현재 파일보다 큰 seek position으로 lseek하면 널문자('\0')으로 빈공간을 채우며 이를 hole이라 함.
	
	write(fd, &ch, 1);
	close(fd);

}
