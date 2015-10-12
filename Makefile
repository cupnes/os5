CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude

.s.o:
	as -o $@ $<
.S.o:
	gcc $(CFLAGS) -o $@ $<
.c.o:
	gcc $(CFLAGS) -o $@ $<

all: fd.img

fd.img: boot.bin sys.bin
	cat boot.bin sys.bin > $@

boot.bin: boot/boot.o
	ld -o $@ $< -T boot/boot.ld

sys.bin: kernel/sys.o kernel/cpu.o kernel/intr.o kernel/excp.o kernel/memory.o kernel/sched.o kernel/timer.o kernel/console_io.o kernel/common.o kernel/debug.o kernel/main.o apps/shell.o apps/uptime_init.o apps/uptime.o
	ld -o $@ kernel/sys.o kernel/cpu.o kernel/intr.o kernel/excp.o kernel/memory.o kernel/sched.o kernel/timer.o kernel/console_io.o kernel/common.o kernel/debug.o kernel/main.o apps/shell.o apps/uptime_init.o apps/uptime.o -Map System.map -s -T kernel/sys.ld -x

boot/boot.o: boot/boot.s

kernel/sys.o: kernel/sys.S

kernel/cpu.o: kernel/cpu.c

kernel/intr.o: kernel/intr.c

kernel/excp.o: kernel/excp.c

kernel/memory.o: kernel/memory.c

kernel/sched.o: kernel/sched.c

kernel/timer.o: kernel/timer.c

kernel/console_io.o: kernel/console_io.c

kernel/common.o: kernel/common.c

kernel/debug.o: kernel/debug.c

kernel/main.o: kernel/main.c

apps/shell.o: apps/shell.c

apps/uptime_init.o: apps/uptime_init.c

apps/uptime.o: apps/uptime.c

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map boot/*~ boot/*.o include/*~ include/*.o kernel/*~ kernel/*.o apps/*~ apps/*.o

run: fd.img
	qemu -fda fd.img
