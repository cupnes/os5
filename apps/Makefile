BIN_DIR = .bin
APP_LD = ../app.ld
APP_DIRS = $(shell find . -maxdepth 1 -type d '!' -iname '.*' '!' -iname 'include')
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -c
CFLAGS += -I../include
CFLAGS += -m32

apps.img: $(APP_DIRS)
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	for appdir in $^; do \
		make -C $$appdir BIN_DIR=../$(BIN_DIR) APP_LD=$(APP_LD) CFLAGS="$(CFLAGS)"; \
	done
	../tools/make_os5_fs.sh $(BIN_DIR)/* > $@

clean:
	rm -rf *~ *.o *.bin *.dat *.img *.map $(BIN_DIR)
	for appdir in $(APP_DIRS); do \
		make -C $$appdir clean; \
	done

.PHONY: clean
