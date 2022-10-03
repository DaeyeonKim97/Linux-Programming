//Shared memory를 이용하기 위한 환경변수
#define MY_ID		4
#define SHM_KEY		(0x9000 + MY_ID)
#define SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)

#define EMPTY_SEM_KEY	(0x5000 + MY_ID)
#define FULL_SEM_KEY	(0x6000 + MY_ID)
#define MUTEX_SEM_KEY	(0x7000 + MY_ID)

// 반복횟수와 buf크기
#define NLOOPS		20
#define MAX_BUF		2

//ItemType struct: int형태의 data를 담고 있음
typedef struct{
	int	data;
}
	ItemType;

//BoundedBufferType struct: MAX_BUF개의 ItemType 배열을 circular queue형태로 가지고 있음.
typedef struct{
	ItemType	buf[MAX_BUF];
	int		in; //데이터가 들어가는 index
	int		out; //데이터를 소모하는 index
	int		counter; //데이터의 개수를 저장
}
	BoundedBufferType;

//Shared Memory의 사이즈를 BoundedBufferType의 사이즈로
#define SHM_SIZE sizeof(BoundedBufferType)
