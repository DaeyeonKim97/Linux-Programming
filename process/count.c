#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	FILE	*src;
	char	ch;
	int 	count = 0;
	
	if (argc !=3) { //입력 형식에 맞지 않을 경우 종료
		fprintf(stderr, "Usage: %s source character\n", argv[0]);
		exit(1);
	}

	if((src = fopen(argv[1], "rt")) == NULL){ //argv로 주어진 파일을 write text모드로 염
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF){ //파일의 모든 문자를 받아옴
		if(ch == argv[2][0]) //문자와 주어진 비교문자를 비교해서 같으면 count ++
			count ++;
	}

	printf("%d\n",count); //같은 문자열의 개수를 출력

	fclose(src); //file close


}
