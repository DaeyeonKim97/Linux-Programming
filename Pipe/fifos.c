#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"


void SigIntHandler(int signo) //signalINT 감지 시 SERV_FIFO를 삭제
{
	if (remove(SERV_FIFO) < 0)  { //SERV_FIFO삭제 실패 시 perror remove
		perror("remove");
		exit(1);
	}

	exit(0);
}


main()
{
	int		fd, cfd, n;
	MsgType		msg;

	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  { //SigInt signal발생 시 SigIntHandler호출
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0)  { //0600(나만write)로 SERV_FIFO를 만듦
		if (errno != EEXIST)  { //이미 존재하는 경우 mkfifo perror
			perror("mkfifo");
			exit(1);
		}
	}

	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  { //mkfifo를 하고나면 special file SERV_FIFO를 열어야됨
		perror("open");
		exit(1);
	}

	while (1)  {
		if ((n= read(fd, (char *)&msg, sizeof(msg)))<0)  { //msg를 기다림
			if (errno == EINTR)  {
				continue;
			} //msg를 발견하였을 경우 빠져나옴
			else  {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s.....", msg.data); //msg의 데이터를 출력

		if ((cfd = open(msg.returnFifo, O_WRONLY))<0)  { //returnFifo이름의 fifo를 open
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid());
		write(cfd, (char *)&msg, sizeof(msg)); // returnFifo로 write
		close(cfd);
		printf("Replied.\n");
	}
}
