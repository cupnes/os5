all: fd.img

fd.img: boot/boot.bin kernel/kernel.bin apps/apps.img
	cat $+ > $@

boot/boot.bin: FORCE
	make -C boot

kernel/kernel.bin: FORCE
	make -C kernel

apps/apps.img: FORCE
	make -C apps

clean:
	make -C boot clean
	make -C kernel clean
	make -C apps clean
	rm -f *~ *.o *.bin *.dat *.img *.map

run: fd.img
	qemu -fda $<

FORCE:
.PHONY: clean FORCE
