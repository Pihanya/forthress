all: build

build: clean forthress.o
	ld -o forthress forthress.o

forthress.o: forthress.asm
	nasm -f elf64 -g -F dwarf forthress.asm -o forthress.o

clean:
	rm -f *.o
	rm -f forthress

run: build
	./forthress

test: build
	./forthress < test.frt
