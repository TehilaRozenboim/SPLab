all: prog

func.o: func.asm
	nasm -g -f elf -o func.o func.asm

task0.o: task0.c
	gcc -g -m32 -c -o task0.o task0.c

prog: task0.o func.o
	gcc -g -m32 task0.o func.o -o prog
