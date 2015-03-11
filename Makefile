AS = as

.s.o:
	$(AS) -o $@ $<
.o.bin:
	objcopy -O binary $< $@

all: fd.img

fd.img: boot.dat sys.dat
	cat boot.dat sys.dat > $@

boot.dat: boot.bin
	cp boot.bin boot.dat

sys.dat: sys.bin
	dd if=/dev/zero of=zero.dat bs=1 count=$$((512 - $$(stat -c%s sys.bin)))
	cat sys.bin zero.dat > $@

boot.bin: boot.o
	ld -o $@ $< -T boot.ld

sys.bin: sys.o
	objcopy -O binary sys.o sys.bin

boot.o: boot.s

sys.o: sys.s

clean:
	rm -f *~ *.o *.bin *.dat *.img

run: fd.img
	qemu -fda fd.img
