KERNEL_PATH = ../kernel
CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude -I$(KERNEL_PATH)/include
CFLAGS	+=	-m32
BIN_DIR = .bin

.c.o:
	gcc $(CFLAGS) -o $@ $<

apps.img: $(BIN_DIR)/shell $(BIN_DIR)/uptime $(BIN_DIR)/whoareyou
	../tools/make_os5_fs.sh $+ > $@

$(BIN_DIR)/shell: shell.o
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	ld -m elf_i386 -o $@ $< -Map shell.map -s -T app.ld -x

$(BIN_DIR)/uptime: uptime.o
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	ld -m elf_i386 -o $@ $< -Map uptime.map -s -T app.ld -x

$(BIN_DIR)/whoareyou: whoareyou.o
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	ld -m elf_i386 -o $@ $< -Map whoareyou.map -s -T app.ld -x

shell.o: shell.c

uptime.o: uptime.c

whoareyou.o: whoareyou.c

clean:
	rm -rf *~ *.o *.bin *.dat *.img *.map $(BIN_DIR)

.PHONY: clean
