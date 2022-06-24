#include <stdio.h>

int strlen_p(char *str) //문자열의 시작pointer를 받아 길이를 출력하는 함수
{
	int len = 0; //문자열의 길이를 0으로 초기화

	while (*str++){ //문자열의 끝(null)이 나올때까지 반복, 반복문을 한번 돌 때마다 다음 문자로 넘어감
		len++; //길이값 +1
	}
	return len; //문자열의 길이를 반환
}

int strlen_a(char str[]) //문자배열을 받아 길이를 반환하는 함수
{
	int i = 0; // i는 길이를 나타내면서 반복문의 iterator을 나타냄

	for (i=0; str[i] != '\0' ; i++); //문자열의 끝(null)이 나올때까지 반복하며 길이값을 +1

	return i; //문자열의 길이를 반환
}

void strcpy_p(char *dst, char *src) //src의 시작주소를 받아 dst에 그 값을 복사
{
	while (*src) { //src의 값이 null이 될 때까지 반복
		*dst++ = *src++; //현재 src pointer의 값을 dst pointer에 복사하고 다음 주소값으로 이동
	}	
	*dst = *src; //마지막 null을 복사
}

void strcpy_a(char dst[], char src[]) //두 문자배열을 받아 dst에 src의 내용을 복사
{
	int i;

	for(i=0; src[i] != '\0'; i++) //src의 내용이 null이 아닐때 까지 반복
		dst[i] = src[i]; //해당 순서의 character를 복사
	dst[i] = src[i]; //마지막 null을 복사
}

void strcat_p(char *dst, char *src) //pointer을 입력받아 dst의 끝에 src의 내용을 복사
{
	while (*dst++); //dst 의 null값까지 pointer 이동
	dst --; //null 이전으로 이동
	while (*src)
		*dst++ = *src++; //src의 null값까지 이동하며 dst에 복사
	*dst = *src; //null 복사
}	

void strcat_a(char dst[], char src[])//문자 배열을 입력받아 dst의 끝에 src의 내용을 복사
{
	int i, j;

	for(i=0; dst[i] !='\0'; i++) ; //dst의 null값의 iterator i
	for(j=0; src[j] !='\0'; j++)
		dst[i+j] = src[j]; // src의 null값 까지 이동하며 dst에 복사
	dst[i+j] = src[j]; //null 복사
}

int strcmp_p(char *dst, char *src) //두 문자열의 pointer을 이용하여 문자열이 같다면 0, dst<src 이면 음수, dst>src 이면 양수를 반환하는 함수
{
	while (*dst) //dst의 값이 null이 나올때까지 반복
	{
		//두 배열의 ASCII값을 비교하여 dst가 크면 1, 작으면 -1을 반환하고 종료
		if(*dst-*src>0){
			//printf("%d",*dst-*src);
			return 1;
		}
		else if(*dst-*src<0){
			//printf("%d",*dst-*src);
			return -1;
		}
		
		dst++; src++; //두 값이 같을경우 다음 iterator로 이동
	}

	if(*src == '\0'){
		return 0;
	} //src도 dst와 같이 종료되었다면 0을 반환
	else{
		return -1;
	} //src의 문자가 더 있을경우 -1을 반환
}

int strcmp_a(char dst[], char src[]) //두 문자배열을 비교하여 문자열이 같다면 0, dst<src 이면 음수, dst>src 이면 양수를 반환하는 함수
{
	int i;

	for (i=0; dst[i] !='\0'; i++)
	{
		//두 배열의 ASCII값을 비교하여 dst가 크면 1, 작으면 -1을 반환하고 종료
		if(dst[i] - src[i] > 0){
			//printf("%d",dst[i]-src[i]);
			return 1;
		}
		else if(dst[i] - src[i] < 0){
			//printf("%d",dst[i]-src[i]);
			return -1;
		}
	}

	if(src[i] == '\0'){
		return 0;
	} //src도 dst와 같이 종료되었다면 0을 반환
	else{
		return -1;
	} //src의 문자가 더 있을경우 -1을 반환

}


main()
{
	int len1, len2;
	int cmp1, cmp2;
	char str1[20], str2[20];
	
	//strlen test
	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p = %d, a= %d\n",len1, len2);
	
	//strcpy test
	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p = %s, a = %s\n", str1, str2);

	//strcat test
	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p = %s, a = %s\n", str1, str2);

	//strcmp test
	//1. 비교문자열이 더 클경우 -1
	cmp1 = strcmp_p(str1, "Hello, Worlk!");
	cmp2 = strcmp_a(str1, "Hello, Worlk!");
	printf("strcmp case-1: p = %d, a = %d\n",cmp1, cmp2);
	//2. 비교문자열이 더 작을경우 1
	cmp1 = strcmp_p(str1, "Hallo, World!");
	cmp2 = strcmp_a(str1, "Hallo, World!");
	printf("strcmp case 1: p = %d, a = %d\n",cmp1, cmp2);
	//3. 두 문자열이 같을 경우 0
	cmp1 = strcmp_p(str1, str2);
	cmp2 = strcmp_a(str1, str2);
	printf("strcmp case 0: p = %d, a = %d\n",cmp1, cmp2);



}
