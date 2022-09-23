#include <stdio.h>

int main(int argc, char* argv[])
{

	int input, square;

	input = atoi(argv[1]); //command-line에서 주어진 정수 값을 string to int
	
	square = input*input; //정수 값을 제곱

	printf("%d\n", square); // 정수 값을 출력
}
