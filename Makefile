AS = as

.s.o:
	$(AS) -o $*.o $<

all: fd.img

fd.img: boot.o signature.dat
	objcopy -O binary boot.o boot.bin
	dd if=/dev/zero of=zero.dat bs=1 count=$$((510 - $$(stat -c%s boot.bin)))
	cat boot.bin zero.dat signature.dat > fd.img

boot.o: boot.s

signature.dat:
	bash -c 'echo -en "\x55\xaa" > signature.dat'

clean:
	rm -f *~ *.o *.bin *.dat *.img

run: fd.img
	qemu -fda fd.img
