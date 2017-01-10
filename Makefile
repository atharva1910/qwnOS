CC = i686-elf-g++
CFLAGS = -c
LDFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

all:
	i686-elf-as boot.s -o boot.o
	$(CC)  $(CFLAGS) kernel.cc -o kernel.o $(LDFLAGS)
	i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

build:
	cp myos.bin isodir/boot/
	grub-mkrescue -o qwnOS.iso isodir

clean:
	rm -rf *.o myos.bin qwnOS.iso
	rm isodir/boot/myos.bin

run:
	qemu-system-i386 -cdrom qwnOS.iso

