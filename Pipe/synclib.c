#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int	Pfd1[2], Pfd2[2];

void TELL_WAIT(void) // Pfd1과 Pfd2 pipe를 만듦
{
	if(pipe(Pfd1) < 0 || pipe(Pfd2) <0){ // 파이프를 둘중 하나라도 만들지 못하면 perror pipe후 종료
		perror("pipe");
		exit(1);
	}
}

void TELL_PARENT(void) // Pfd2의 pipe에 c를 write 
{
	if(write(Pfd2[1], "c", 1) != 1){ //실패 시 perror write후 종료
		perror("write");
		exit(1);
	}
}

void WAIT_PARENT(void) // Pfd1의 pipe에서 character를 읽어옴
{
	char	c;
	if (read(Pfd1[0], &c, 1) != 1){ //pipe에서 character를 읽어오지 못했을 경우 perror read
		perror("read");
		exit(1);
	}
	if (c != 'p'){ //pipe에서 읽어온 character가 p가 아닐경우 stderr WAITPARENT
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

void TELL_CHILD(void) // Pfd1의 pipe에 p를 씀
{
	if(write(Pfd1[1], "p", 1) != 1){ //pipe에 p를 쓰지 못했을 경우 perror write
		perror("write");
		exit(1);
	}
}

void WAIT_CHILD(void) // Pfd2의 pipe에서 character를 읽어옴
{
	char	c;
	if (read(Pfd2[0], &c, 1) != 1){ //Pfd2의 pipe에서 character를 읽어오지 못했을 경우 perror read
		perror("read");
		exit(1);
	}
	if(c != 'c'){ //Pfd2의 pipe에서 읽어온 character가 c가 아닐경우 stderr WAITCHILD후 종료
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}

