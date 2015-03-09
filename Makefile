AS = as

.s.o:
	$(AS) -o $@ $<
.o.bin:
	objcopy -O binary $< $@

all: fd.img

fd.img: boot.dat sys.dat
	cat boot.dat sys.dat > $@

boot.dat: boot.bin signature.dat
	dd if=/dev/zero of=zero.dat bs=1 count=$$((510 - $$(stat -c%s boot.bin)))
	cat boot.bin zero.dat signature.dat > $@

sys.dat: sys.bin
	dd if=/dev/zero of=zero.dat bs=1 count=$$((512 - $$(stat -c%s sys.bin)))
	cat sys.bin zero.dat > $@

boot.bin: boot.o
	objcopy -O binary boot.o boot.bin

sys.bin: sys.o
	objcopy -O binary sys.o sys.bin

boot.o: boot.s

sys.o: sys.s

signature.dat:
	bash -c 'echo -en "\x55\xaa" > signature.dat'

clean:
	rm -f *~ *.o *.bin *.dat *.img

run: fd.img
	qemu -fda fd.img
