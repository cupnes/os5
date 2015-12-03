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

fd.img: boot/boot.bin sys.bin shell.bin uptime.bin
	cat boot/boot.bin sys.bin shell.bin uptime.bin > $@

boot/boot.bin: FORCE
	make -C boot

shell.bin: apps/shell.o
	ld -o $@ $< -Map apps/shell.map -s -T apps/app.ld -x

uptime.bin: apps/uptime.o
	ld -o $@ $< -Map apps/uptime.map -s -T apps/app.ld -x

sys.bin: kernel/sys.o kernel/cpu.o kernel/intr.o kernel/excp.o kernel/memory.o kernel/sched.o kernel/timer.o kernel/console_io.o kernel/debug.o kernel/main.o kernel/kern_task_init.o apps/shell_init.o apps/uptime_init.o
	ld -o $@ kernel/sys.o kernel/cpu.o kernel/intr.o kernel/excp.o kernel/memory.o kernel/sched.o kernel/timer.o kernel/console_io.o kernel/debug.o kernel/main.o kernel/kern_task_init.o apps/shell_init.o apps/uptime_init.o -Map System.map -s -T kernel/sys.ld -x

kernel/sys.o: kernel/sys.S

kernel/cpu.o: kernel/cpu.c

kernel/intr.o: kernel/intr.c

kernel/excp.o: kernel/excp.c

kernel/memory.o: kernel/memory.c

kernel/sched.o: kernel/sched.c

kernel/timer.o: kernel/timer.c

kernel/console_io.o: kernel/console_io.c

kernel/debug.o: kernel/debug.c

kernel/main.o: kernel/main.c

kernel/kern_task_init.o: kernel/kern_task_init.c

apps/shell_init.o: apps/shell_init.c

apps/uptime_init.o: apps/uptime_init.c

apps/shell.o: apps/shell.c

apps/uptime.o: apps/uptime.c

clean:
	make -C boot clean
	rm -f *~ *.o *.bin *.dat *.img *.map include/*~ include/*.o kernel/*~ kernel/*.o apps/*~ apps/*.o

run: fd.img
	qemu -fda fd.img

FORCE:
.PHONY: clean FORCE
