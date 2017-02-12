LIB_DIR = .lib
BIN_DIR = .bin
APP_LD = ../app.ld
LIB_DIRS = libkernel libcommon libconsole libstring
APP_DIRS = $(shell find . -maxdepth 1 -type d '!' -iname '.*' '!'	\
-iname 'include' '!' -iname 'lib*')
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -c
CFLAGS += -I../include
CFLAGS += -m32
CFLAGS += -DCOMPILE_APP
LDFLAGS = -L../$(LIB_DIR)
LIBS = -lstring -lconsole -lcommon -lkernel

apps.img: lib app
	../tools/make_os5_fs.sh $(BIN_DIR)/* > $@

lib: $(LIB_DIRS)
	[ -d $(LIB_DIR) ] || mkdir $(LIB_DIR)
	for libdir in $^; do \
		make -C $$libdir LIB_DIR=../$(LIB_DIR)	\
		APP_LD=$(APP_LD) CFLAGS="$(CFLAGS)"	\
		LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)"; \
	done

app: $(APP_DIRS)
	[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)
	for appdir in $^; do \
		make -C $$appdir BIN_DIR=../$(BIN_DIR)	\
		APP_LD=$(APP_LD) CFLAGS="$(CFLAGS)"	\
		LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)"; \
	done

clean:
	rm -rf *~ *.o *.a *.bin *.dat *.img *.map $(LIB_DIR) $(BIN_DIR)
	for dir in $(LIB_DIRS) $(APP_DIRS); do \
		make -C $$dir clean; \
	done

.PHONY: lib app clean
