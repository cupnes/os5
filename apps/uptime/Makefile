NAME=uptime

$(BIN_DIR)/$(NAME): $(NAME).o
	ld -m elf_i386 -o $@ $< -Map $(NAME).map -s -T $(APP_LD) -x $(LDFLAGS) $(LIBS)

%.o: %.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf *~ *.o *.map

.PHONY: clean
