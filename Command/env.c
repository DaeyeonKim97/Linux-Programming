#include <stdio.h>

main(int argc, char *argv[], char *envp[])
{
	int		i;
	char		**p;
	extern char	**environ; //extern을 통해 다른 소스파일의 전역변수를 사용

	printf("List command-line arguments\n");
	for(i=0; i<argc; i++)
	{
		printf("%s\n", argv[i]);
	} //command line parameter로 주어진 argument들을 출력

	printf("\n");
	printf("List enviroment variables from environ variable\n");
	for(p = environ; *p != NULL ; p++)
	{
		printf("%s\n", *p);
	} //enviroment로 주어진 var들을 출력

	printf("\n");
	printf("List environment variables from envp variable\n");
	for(p = envp; *p != NULL; p++){
		printf("%s\n", *p);
	} //envp로 주어진 var들을 출력
}
