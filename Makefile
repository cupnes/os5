KERNEL_PATH = kernel
CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude -I$(KERNEL_PATH)/include

.c.o:
	gcc $(CFLAGS) -o $@ $<

all: fd.img

fd.img: boot/boot.bin kernel/kernel.bin shell.bin uptime.bin
	cat $+ > $@

boot/boot.bin: FORCE
	make -C boot

kernel/kernel.bin: FORCE
	make -C kernel

shell.bin: apps/shell.o
	ld -o $@ $< -Map apps/shell.map -s -T apps/app.ld -x

uptime.bin: apps/uptime.o
	ld -o $@ $< -Map apps/uptime.map -s -T apps/app.ld -x

apps/shell.o: apps/shell.c

apps/uptime.o: apps/uptime.c

clean:
	make -C boot clean
	make -C kernel clean
	rm -f *~ *.o *.bin *.dat *.img *.map include/*~ include/*.o apps/*~ apps/*.o

run: fd.img
	qemu -fda $<

FORCE:
.PHONY: clean FORCE
