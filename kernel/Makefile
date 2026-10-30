CC      = gcc
AS      = nasm
LD      = ld
CFLAGS  = -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T link.ld

OBJS = boot.o gdt_flush.o isr.o gdt.o tss.o idt.o kernel.o

all: mykernel.elf

boot.o: boot.S
	$(AS) -f elf32 boot.S -o boot.o

gdt_flush.o: gdt_flush.S
	$(AS) -f elf32 gdt_flush.S -o gdt_flush.o

isr.o: isr.S
	$(AS) -f elf32 isr.S -o isr.o

gdt.o: gdt.c gdt.h tss.h
	$(CC) $(CFLAGS) -c gdt.c -o gdt.o

tss.o: tss.c tss.h gdt.h
	$(CC) $(CFLAGS) -c tss.c -o tss.o

idt.o: idt.c idt.h gdt.h
	$(CC) $(CFLAGS) -c idt.c -o idt.o

kernel.o: kernel.c gdt.h idt.h tss.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

mykernel.elf: $(OBJS) link.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

iso: mykernel.elf
	mkdir -p iso/boot/grub
	cp mykernel.elf iso/boot/
	printf 'set timeout=0\nset default=0\nmenuentry "kernel" {\n  multiboot /boot/mykernel.elf\n  boot\n}\n' > iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso

run: iso
	qemu-system-i386 -cdrom kernel.iso

clean:
	rm -f *.o mykernel.elf kernel.iso
	rm -rf iso
