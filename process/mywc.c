#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[])
{	
	pid_t	pid; //프로세스 아이디를 저장할 변수

	char* argvs[argc+2]; //execvp에 argv로 들어갈 string배열
	for(int i = 1; i < argc; i++){
		argvs[i+1] = argv[i];
	} //argv의 파일명들을 받아옴
	argvs[argc+1] = NULL; //마지막을 null로 설정
	argvs[0] = "wc"; //첫 번째 인자를 wc로 설정
	argvs[1] = "-w"; //두 번째 인자에 -w옵션 설정


	if((pid = fork())< 0){ //현재 프로세스를 fork
		perror("fork");
		exit(1); //실패 시 종료
	}

	else if (pid == 0) {
		/*child*/

		if(execvp("wc", argvs)<0){ //child process에서 execvp를 통해 wc -w 명령을 수행
			perror("execvp");
			exit(1); //실패 시 종료
		}
	}

	if(waitpid(pid, NULL, 0) < 0){
		perror("waitpid");
		exit(1); //parent에서는 child의 종료 시 까지 대기 후 진행
	}

}
