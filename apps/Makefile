KERNEL_PATH = ../kernel
CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude -I$(KERNEL_PATH)/include
CFLAGS	+=	-m32
BIN_DIR = .bin
APP_LD = ../app.ld

.c.o:
	gcc $(CFLAGS) -o $@ $<

apps.img: $(BIN_DIR)/shell $(BIN_DIR)/uptime whoareyou
	mv $(BIN_DIR)/shell $(BIN_DIR)/0shell
	../tools/make_os5_fs.sh $(BIN_DIR)/* > $@

$(BIN_DIR)/shell: shell.o
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	ld -m elf_i386 -o $@ $< -Map shell.map -s -T app.ld -x

$(BIN_DIR)/uptime: uptime.o
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	ld -m elf_i386 -o $@ $< -Map uptime.map -s -T app.ld -x

whoareyou:
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	make -C $@ BIN_DIR=../$(BIN_DIR) KERNEL_PATH=../$(KERNEL_PATH) APP_LD=$(APP_LD)

shell.o: shell.c

uptime.o: uptime.c

clean:
	rm -rf *~ *.o *.bin *.dat *.img *.map $(BIN_DIR)
	make -C whoareyou clean

.PHONY: whoareyou clean
