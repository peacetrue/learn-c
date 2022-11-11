all: clean main
main:  main.o
	gcc -o main main.o
main.o: main.s
	gcc -c -g -o main.o main.s
main.s: main.i
	gcc -S -g -o main.s main.i
main.i: main.c
	gcc -E -g -o main.i main.c
clean:
	rm -rf main.i main.s main.o main
