CROSS = aarch64-linux-gnu-
CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -c
OBJS = boot.o main.o
NAME = kernel8

$(NAME).img: $(NAME).elf
	$(OBJCOPY) -O binary $< $@

$(NAME).elf: $(OBJS)
	$(LD) -o $@ $+ -Map $(NAME).map -s -T rpi3.ld -x

.S.o:
	$(CC) $(CFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *~ *.o *.elf *.map *.img

.PHONY: clean
