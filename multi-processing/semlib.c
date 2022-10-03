#include <stdio.h>
#include "semlib.h"
 
int semInit(key_t key) //semaphore를 하나 만들고 semaphore의 id를 반환해줌
{
    int     semid;

    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  { //세마포어 갯수를 1개로, 없다면 새로 생성하는 옵션으로 만듦
		perror("semget");
        return -1; //실패 시 -1 return
    }

    return semid;
}
 
int semInitValue(int semid, int value) //semaphore의 초기값을 지정해줌.
{
    union semun {
            int     val;
    } semun;

    semun.val = value; //union의 val 값을 value로 설정
    if (semctl(semid, 0, SETVAL, semun) < 0)  { //semid로 식별되는 semaphore set의 0번째 배열의 값을 semun으로 설정
		perror("semctl"); 
        return -1; //실패 시 perror semctl, -1을 반환
    }
 
    return semid;
}
 
int semWait(int semid) //semop를 이용한 semaphore wait
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0; //semaphore를 하나만 만들기 때문에 0번 (세마포어 번호를 0번으로 설정)
    semcmd.sem_op = -1; //wait이므로 -1 (세마포어 증감 값을 -1로 설정, 사용하기 전 세마포어 값을 감소)
    semcmd.sem_flg = SEM_UNDO; //프로세스가 종료되면 시스템 세마포어 설정을 원래 상태로 되돌림(기본 값)
    if (semop(semid, &semcmd, 1) < 0)  { //세마포어의 값을 변경
		perror("semop");
        return -1; //실패 시 perror semop, -1을 반환
    }
 
    return 0;
}
 
int semTryWait(int semid) //세마포어의 값이 0이면 기다려야 되는데 기다리지 않음
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO; //flg에 IPC_NOWAIT을 할 경우 기다리지 않고 0일 때 fail로 리턴
    if (semop(semid, &semcmd, 1) < 0)  {//세마포어의 값을 변경
		perror("semop");
        return -1; //실패 시 perror semop, -1을 반환
    }
 
    return 0;
}

int semPost(int semid) //세마포어를 사용 후에 반납하는 함수
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = 1; //세마포어 증감값을 +1로 설정, 사용하고 난 후 1증가
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int semGetValue(int semid) //현재의 값을 가져오는 함수
{
    union semun {
            int     val;
    } dummy;
 
    return semctl(semid, 0, GETVAL, dummy); //semctl GETVAL을 이용하여 세마포어 0번째 값을 dummy를 통해 가져옴
}
 
int semDestroy(int semid) //세마포어를 다 했을 경우 삭제
{
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  { //IPC_RMID옵션을 통해 semaphore를 삭제함
		perror("semctl");
        return -1;
    }
    close(semid); //semid를 close
 
    return 0;
}
