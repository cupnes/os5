CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude

.s.o:
	as -o $@ $<
.c.o:
	gcc $(CFLAGS) -o $@ $<

all: fd.img

fd.img: boot.bin sys.bin
	cat boot.bin sys.bin > $@

boot.bin: boot/boot.o
	ld -o $@ $< -T boot/boot.ld

sys.bin: kernel/sys.o kernel/intr.o lib/console_io.o kernel/main.o
	ld -o $@ kernel/sys.o kernel/intr.o lib/console_io.o kernel/main.o -M -s -T kernel/sys.ld -x > System.map

boot/boot.o: boot/boot.s

kernel/sys.o: kernel/sys.s

kernel/intr.o: kernel/intr.c

lib/console_io.o: lib/console_io.c

kernel/main.o: kernel/main.c

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map boot/*~ boot/*.o include/*~ include/*.o lib/*~ lib/*.o kernel/*~ kernel/*.o

run: fd.img
	qemu -fda fd.img
