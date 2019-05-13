all: foo

foo: foo.o
	gcc -o foo foo.o
foo.o: foo.c
	gcc -c foo.c

clean:
	rm foo foo.o

