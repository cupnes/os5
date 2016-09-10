KERNEL_PATH = ../kernel
CFLAGS	=	-Wall -Wextra
CFLAGS	+=	-nostdinc -nostdlib -fno-builtin -c
CFLAGS	+=	-Iinclude -I$(KERNEL_PATH)/include
CFLAGS	+=	-m32
BIN_DIR = .bin
APP_LD = ../app.ld

.c.o:
	gcc $(CFLAGS) -o $@ $<

apps.img: 0shell uptime whoareyou
	../tools/make_os5_fs.sh $(BIN_DIR)/* > $@

0shell:
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	make -C $@ BIN_DIR=../$(BIN_DIR) KERNEL_PATH=../$(KERNEL_PATH) APP_LD=$(APP_LD)

uptime:
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	make -C $@ BIN_DIR=../$(BIN_DIR) KERNEL_PATH=../$(KERNEL_PATH) APP_LD=$(APP_LD)

whoareyou:
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	make -C $@ BIN_DIR=../$(BIN_DIR) KERNEL_PATH=../$(KERNEL_PATH) APP_LD=$(APP_LD)

clean:
	rm -rf *~ *.o *.bin *.dat *.img *.map $(BIN_DIR)
	make -C 0shell clean
	make -C uptime clean
	make -C whoareyou clean

.PHONY: 0shell uptime whoareyou clean
