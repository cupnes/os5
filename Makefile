all: fd.img

fd.img: boot/boot.bin kernel/kernel.bin apps/apps.img
	cat $+ > $@

boot/boot.bin:
	make -C boot

kernel/kernel.bin:
	make -C kernel

apps/apps.img:
	make -C apps

clean:
	make -C boot clean
	make -C kernel clean
	make -C apps clean
	rm -f *~ *.o *.bin *.dat *.img *.map

run: fd.img
	qemu-system-i386 -fda $<

.PHONY: boot/boot.bin kernel/kernel.bin apps/apps.img clean
