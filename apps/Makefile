KERNEL_PATH = ../kernel
CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude -I$(KERNEL_PATH)/include

.c.o:
	gcc $(CFLAGS) -o $@ $<

apps.img: shell.bin uptime.bin
	cat $+ > $@

shell.bin: shell.o
	ld -o $@ $< -Map shell.map -s -T app.ld -x

uptime.bin: uptime.o
	ld -o $@ $< -Map uptime.map -s -T app.ld -x

shell.o: shell.c

uptime.o: uptime.c

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

.PHONY: clean
