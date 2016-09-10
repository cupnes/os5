NAME=0shell
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -c
CFLAGS += -Iinclude -I$(KERNEL_PATH)/include
CFLAGS += -m32

$(BIN_DIR)/$(NAME): $(NAME).o
	ld -m elf_i386 -o $@ $< -Map $(NAME).map -s -T $(APP_LD) -x

%.o: %.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf *~ *.o *.map

.PHONY: clean
