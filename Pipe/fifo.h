#define	MAX_FIFO_NAME	32

#define	SERV_FIFO	"./.fifo"

typedef struct  {
	char	returnFifo[MAX_FIFO_NAME]; //server가 이 fifo를 이용해서 message 전달
	char	data[128];
}
	MsgType;
