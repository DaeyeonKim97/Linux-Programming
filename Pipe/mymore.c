#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int 
main() 
{
	int fd[2];
	pid_t child;

	if(pipe(fd) == -1)  { //Pipe실패시 perror pipe출력 후 종료
		perror("pipe");
		exit(1);
	}
 
	child = fork(); //프로세스를 fork하여 child process를 만듦
	if(child == (pid_t)(-1))  { //fork실패 시 perror fork출력 후 종료
        	perror("fork");
		exit(1);
	}
	else if(child == (pid_t)0) { //child process의 경우
        	close(1); //standard output close , pipe 로 연결해줄것이기 때문에
		close(fd[0]); // pipe의 read방향 close
        	
        	if(dup(fd[1]) == -1)  { //standard output이 다른process의 standard input으로 들어감
					//dup은 close한 fd[1]과 1번을 duplicate
        		perror("dup");
			exit(1);
		}

        	if((execlp("ls", "ls", "-l", NULL)) == -1)  { //ls -l의 출력이 pipe를 통해 전달
        		perror("execlp");
			exit(1);
		}
	} 
	else {
        	close(0); //standard input close , pipe로 연결해줄 것이기 때문에
		close(fd[1]); // pipe의 write방향 close
        	
        	if(dup2(fd[0],0) == -1)  { //dup2는 duplicate할 것을 지정해줌
        		perror("dup");
			exit(1);
		}

        	if((execlp("more", "more", NULL)) == -1)  { // more의 input으로 ls -l이 전달
        		perror("execlp");
			exit(1);
		}
	}
 
	return 0;
}
