CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -fno-common -c
CFLAGS += -Iinclude
ifneq ($(x86_64),true)
CFLAGS += -m32
else
CFLAGS += -DX86_64
endif
LDFLAGS = -Map System.map -s -x
ifneq ($(x86_64),true)
LDFLAGS += -m elf_i386 -T sys.ld
OBJS = sys.o cpu.o intr.o excp.o memory.o sched.o fs.o task.o	\
		syscall.o lock.o timer.o console_io.o queue.o	\
		common.o debug.o init.o kern_task_init.o
else
LDFLAGS += -T sys_64.ld
OBJS = init_64.o fb.o font.o fbcon.o kbc.o cpu.o excp.o intr.o sys_64.o
endif

kernel_64.bin: $(OBJS)
	ld $(LDFLAGS) -o $@ $+

kernel.bin: $(OBJS)
	ld $(LDFLAGS) -o $@ $+

%.o: %.S
	gcc $(CFLAGS) -o $@ $<
%.o: %.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

.PHONY: clean
