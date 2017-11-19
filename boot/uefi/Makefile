CC = x86_64-w64-mingw32-gcc
OBJCOPY = x86_64-w64-mingw32-objcopy
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin
CFLAGS += -Wl,--subsystem,10

image.efi: boot.o libuefi/libuefi.a kernel.o apps.o
	$(CC) $(CFLAGS) -e efi_main -o $@ $+

boot.o: boot.c
	$(CC) $(CFLAGS) -Iinclude -c -o $@ $<

libuefi/libuefi.a:
	make -C libuefi CC=$(CC) CFLAGS="$(CFLAGS)"

kernel.o: $(KERNEL)
	cp $(KERNEL) kernel.bin
	$(OBJCOPY) -I binary -O pe-x86-64 -B i386:x86-64 kernel.bin $@

apps.o: $(APPS)
	cp $(APPS) apps.img
	$(OBJCOPY) -I binary -O pe-x86-64 -B i386:x86-64 apps.img $@

clean:
	rm -f *~ *.o *.bin *.img
	make -C libuefi clean

.PHONY: clean
