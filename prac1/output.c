#include <stdio.h>

int main()
{
	char c = 'a' , s[] = "hello"; //character c, character list s
	int i = 100; long l = 99999; //integer i, long integer l
	float f = 3.14; double d = 99.999; //float f, double d
	int *p = &i; // integer pointer p is reference of i
	
	printf("Output : %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
	//print char, string, integer, hexadeciaml integer preeceeded with 0 (uppercase), long integer, 
	//float with 4 digits after decimal point, 2digits after decimal point, pointer
	putchar(c); //print character
	puts(s);    //print string
}
