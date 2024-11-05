CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s

all: kernel.img

kernel.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel.img

kernel.elf: boot.o main.o
	$(CC) $(CFLAGS) -T linker.ld -o kernel.elf boot.o main.o

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o *.elf 
