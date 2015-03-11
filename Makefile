.s.o:
	as -o $@ $<

all: fd.img

fd.img: boot.bin sys.bin
	cat boot.bin sys.bin > $@

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
