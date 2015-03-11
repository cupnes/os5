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
	cp $< $@

boot.bin: boot.o
	ld -o $@ $< -T boot.ld

sys.bin: sys.o
	ld -o $@ $< -T sys.ld

boot.o: boot.s

sys.o: sys.s

clean:
	rm -f *~ *.o *.bin *.dat *.img

run: fd.img
	qemu -fda fd.img
