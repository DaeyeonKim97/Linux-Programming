#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int argc, char *argv[])
{
	int		fdin, fdout;
	char		*src, *dst;
	struct stat	statbuf;

	if (argc != 3)  { //입력 형식에 맞지 않을경우 입력형실 출력 후 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0)  { //copy할 source를 read only로 엶
		perror("open");
		exit(1);
	}
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  { //copy할 destination을 write하기 위해 염
		perror("open");
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0)  { //fdin의 파일정보를 읽어옴
		perror("fstat");
		exit(1);
	}
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  { //fdout의 처음에서 fdin의 statbuf만큼 이동
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);

	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) 
		== (caddr_t) -1)  { //memory mapped file을 사용하여 src라는 virtual address에 내용을 복사
		perror("mmap");
		exit(1);
	}
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  { //mmap을 이용하여 다른process와 share하여 fdout file descriptor에 쓸거다
		perror("mmap");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size); //src에서 읽어서 st_size만큼 dst에 복사.

	close(fdin);
	close(fdout); //file close
}
