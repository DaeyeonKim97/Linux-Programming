#include <stdio.h>

main(int argc, char *argv[])
{
	int i;
	
	for(i=0 ; i<argc; i++) //arguments 갯수만큼 반복
		printf("argv[%d]=%s\n", i, argv[i]); //arguments를 순서대로 출력
}
