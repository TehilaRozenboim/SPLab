char2ascii: char2ascii.o
	gcc -g -m32 -Wall -o char2ascii char2ascii.o
	rm char2ascii.o

char2ascii.o: task1a.c
	gcc -g -m32 -Wall -c -o char2ascii.o task1a.c

.PHONY: clean
	rm -rf ./*.o char2ascii