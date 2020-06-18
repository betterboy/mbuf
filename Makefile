all: test

test: test.c mbuf.c
	gcc -g -o test $^

clean:
	rm test