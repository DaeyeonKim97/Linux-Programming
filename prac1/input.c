#include <stdio.h>

main()
{
	char c, s[80];
	int i; long l;
	float f; double d;

	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d);
	//주소에 입력을 받음. 문자열은 그자체로 문자열의 시작주소
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);

	c = getchar(); // character을 입력받음
	putchar(c);

	gets(s); // string문자열을 입력받음
	puts(s);
}
