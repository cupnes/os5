CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude

.S.o:
	gcc $(CFLAGS) -o $@ $<
.c.o:
	gcc $(CFLAGS) -o $@ $<

kernel.bin: sys.o cpu.o intr.o excp.o memory.o sched.o timer.o console_io.o debug.o main.o kern_task_init.o shell_init.o uptime_init.o
	ld -o $@ $+ -Map System.map -s -T sys.ld -x

sys.o: sys.S

cpu.o: cpu.c

intr.o: intr.c

excp.o: excp.c

memory.o: memory.c

sched.o: sched.c

timer.o: timer.c

console_io.o: console_io.c

debug.o: debug.c

main.o: main.c

kern_task_init.o: kern_task_init.c

shell_init.o: shell_init.c

uptime_init.o: uptime_init.c

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

.PHONY: clean
