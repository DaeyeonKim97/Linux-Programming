#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"


main()
{
	char		fname[MAX_FIFO_NAME];
	int			fd, sfd, n;
	MsgType		msg;

	sprintf(fname, ".fifo%d", getpid()); //client specific한 fifo를 만듦 (.fifo+pid)
	if (mkfifo(fname, 0600) < 0)  { //나만write하는 fifo를 만듦
		perror("mkfifo");
		exit(1);
	}

	if ((fd = open(fname, O_RDWR)) < 0)  { //내가 기다릴 fifo를 open
		perror("open");
		exit(1);
	}
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  { //server에 data를 보낼 fifo를 open
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname); //msg를 만드는 작업
	sprintf(msg.data, "This is a request from %d.", getpid()); //msg를 만드는 작업
	write(sfd, (char*)&msg, sizeof(msg)); //server fifo에 write를 해서 전달
	printf("Sent a request.....");

	if ((n = read(fd, (char*)&msg, sizeof(msg)))<0)  { //내가 만든 fifo에서 msg를 기다림
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data); //도착하면 msg.data출력

	close(fd);
	close(sfd); //file discriptor 출력

	/* Remove temporary FIFO */
	if (removd(fname)<0)  {
		perror("remove");
		exit(1);
	}
}
