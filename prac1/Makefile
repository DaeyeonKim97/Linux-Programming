CC = gcc
CFLAGS =
LDFLAGS =

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = hello output input args string

all : $(ALL)

hello :hello.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL)