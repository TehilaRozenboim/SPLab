char2ascii: task2.o
	gcc -g -m32 -Wall -o char2ascii task2.o
	rm task2.o

task2.o: task2.c
	gcc -g -m32 -Wall -c -o task2.o task2.c

.PHONY: clean
	rm -rf ./*.o main