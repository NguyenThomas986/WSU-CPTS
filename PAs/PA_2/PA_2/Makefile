CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic

scheduler: scheduler.c
	$(CC) $(CFLAGS) scheduler.c -o scheduler

test01:
	./scheduler sample_io/input/input-1

test02:
	./scheduler sample_io/input/input-2

test03:
	./scheduler sample_io/input/input-3

clean:
	rm -f scheduler *.o *~
