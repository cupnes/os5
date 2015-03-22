CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude

.s.o:
	as -o $@ $<

all: fd.img

fd.img: boot.bin sys.bin
	cat boot.bin sys.bin > $@

boot.bin: boot/boot.o
	ld -o $@ $< -T boot/boot.ld

sys.bin: kernel/sys.o lib/console_io.o kernel/main.o
	ld -o $@ kernel/sys.o lib/console_io.o kernel/main.o -M -s -T kernel/sys.ld -x > System.map

boot/boot.o: boot/boot.s

kernel/sys.o: kernel/sys.s

lib/console_io.o: lib/console_io.c include/io_port.h include/console_io.h
	gcc $(CFLAGS) -o $@ $<

kernel/main.o: kernel/main.c include/console_io.h
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

run: fd.img
	qemu -fda fd.img
