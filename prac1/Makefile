CC = gcc
CFLAGS =
LDFLAGS =

# <Target> : <Dependencies>
# <Recipe>

# Target: 빌드 대상 이름. 통상 이 Rule에서 최종적으로 생성해내는 파일명을 써 줍니다.
# Dependencies: 빌드 대상이 의존하는 Target이나 파일 목록. 여기에 나열된 대상들을 먼저 만들고 빌드 대상을 생성합니다.
# Recipe: 빌드 대상을 생성하는 명령. 여러 줄로 작성할 수 있으며, 각 줄 시작에 반드시 Tab문자로 된 Indent가 있어야 합니다.

# Built-in rule : 굳이 기술하지 않아도 *.c를 *.o로 바꾸어줌.
# 				  그러나 헤더파일 등의 의존성을 검사하지 못함 -> Increment build 불가 하므로 기술해야 함. 

# Automatic Variables : '$@';타겟이름  					'$<';첫 번째 prerequisite 
#						'$^';prerequisite의 전체 목록	 '$?';prerequisite 중 변경된 것들의 목록


.SUFFIXES : .c .o

.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = hello output input args string

all : $(ALL)

# make hello는 hello.o에 종속성이 있다.
# gcc -o hello.o hello
hello : hello.o
	$(CC) -o $@ $< $(LDFLAGS)

output : output.o
	$(CC) -o $@ $< $(LDFLAGS)

input : input.o
	$(CC) -o $@ $< $(LDFLAGS)

# .o file과 실행파일을 모두 삭제함.
clean :
	rm -rf *.o $(ALL)