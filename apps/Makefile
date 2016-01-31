KERNEL_PATH = ../kernel
CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude -I$(KERNEL_PATH)/include

.c.o:
	gcc $(CFLAGS) -o $@ $<

apps.img: bin/shell bin/uptime
	../tools/make_os5_fs.sh $+ > $@

bin/shell: shell.o
	[ -d bin ] || mkdir bin
	ld -o $@ $< -Map shell.map -s -T app.ld -x

bin/uptime: uptime.o
	[ -d bin ] || mkdir bin
	ld -o $@ $< -Map uptime.map -s -T app.ld -x

shell.o: shell.c

uptime.o: uptime.c

clean:
	rm -rf *~ *.o *.bin *.dat *.img *.map bin

.PHONY: clean
