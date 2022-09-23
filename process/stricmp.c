#include <stdio.h>

int stricmp_p(char *dst, char *src) //compare dst, src 대소문자의 구분 없이
{
	while (*dst) //dst의 모든 문자에 대해서 반복
	{
		char d,s;
		if(*dst >='a' && *dst <= 'z'){ //dst의 문자가 소문자일 경우 대문자로 바꿔줌
			d = *dst - 32; 
		}
		else{
			d = *dst;
		}

		if(*src >='a' && *src <= 'z'){ //src의 문자가 소문자일 경우 대문자로 바꿔줌
			s = *src - 32; 
		}
		else{
			s = *src;
		}

		if(d-s>0){ //dst의 문자가 src의 문자보다 클 경우 1을 반환하고 종료
			//printf("%d",*dst-*src);
			return 1;
		}
		else if(d-s<0){ //src의 문자가 dst의 문자보다 클 경우 -1을 반환하고 종료
			//printf("%d",*dst-*src);
			return -1;
		}
		
		dst++; src++; //다음 문자로 넘어감
	}

	if(*src == '\0'){ //dst의 종료시에 src도 종료되었을 경우 같은 문자열로 판별하고 0을 반환
		return 0;
	} 
	else{ //src의 문자열의 길이가 더 클경우 -1을 반환
		return -1;
	} 
}

void main()
{
	char str1[] = "Hello";
	char str2[] = "hello";
	char str3[] = "World!";
	printf("%d : %d\n", stricmp_p(str1, str2),stricmp_p(str1, str3));
}
