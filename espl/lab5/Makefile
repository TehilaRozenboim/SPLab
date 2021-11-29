%.o: %.s
	nasm -g -O1 -f elf -o $@ $<

%.o: %.c
	gcc -m32 -g -nostdlib -fno-stack-protector -c -o $@ $<

all: lwca task2a task2b

lwca: lwc.o task1b.o
	ld  -melf_i386 -o $@ $^

task2a: task2a.o
	ld -melf_i386 -o task2a task2a.o

task2b: task2b.o
	ld -melf_i386 -o task2b task2b.o


clean:
	rm -f *.o lwca
	rm -f *.o task2a
	rm -f *.o task2b
