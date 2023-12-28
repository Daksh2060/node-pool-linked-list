all: test

test: 
	gcc -Wall -Wextra -g test.c -o test

clean:
	rm -f test *.o