CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -c
CFLAGS += -Iinclude
CFLAGS += -m32
LDFLAGS = -Map System.map -s -x
LDFLAGS += -m elf_i386 -T sys.ld
OBJS = sys.o cpu.o intr.o excp.o memory.o sched.o fs.o task.o	\
		syscall.o lock.o timer.o console_io.o queue.o	\
		common.o debug.o init.o kern_task_init.o

kernel.bin: $(OBJS)
	ld $(LDFLAGS) -o $@ $+

%.o: %.S
	gcc $(CFLAGS) -o $@ $<
%.o: %.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

.PHONY: clean
