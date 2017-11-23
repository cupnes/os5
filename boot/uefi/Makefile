CC = x86_64-w64-mingw32-gcc
OBJCOPY = x86_64-w64-mingw32-objcopy
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin
CFLAGS += -Wl,--subsystem,10

boot.efi: boot.o libuefi/libuefi.a
	$(CC) $(CFLAGS) -e efi_main -o $@ $+

boot.o: boot.c
	$(CC) $(CFLAGS) -Iinclude -c -o $@ $<

libuefi/libuefi.a:
	make -C libuefi CC=$(CC) CFLAGS="$(CFLAGS)"

clean:
	rm -f *~ *.o
	make -C libuefi clean

.PHONY: clean
