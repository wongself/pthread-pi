clean:
	rm *.c~ *.c# Makefile~ pthreadPI
monte:
	gcc -o pthreadPI.o pthreadMonte.c -lm -pthread
nila:
	gcc -o pthreadPI.o pthreadNilakantha.c -lm -pthread
run:
	./pthreadPI.o
