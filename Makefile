all: fd.img

fd.img: boot/legacy_bios/boot.bin kernel/kernel.bin apps/apps.img
	cat $+ > $@

boot/legacy_bios/boot.bin:
	make -C boot/legacy_bios

kernel/kernel.bin:
	make -C kernel

apps/apps.img:
	make -C apps

doc:
	make -C doc

clean:
	make -C boot/legacy_bios clean
	make -C kernel clean
	make -C apps clean
	make -C doc clean
	rm -f *~ *.o *.bin *.dat *.img *.map

run: fd.img
	qemu-system-i386 -fda $<

.PHONY: boot/legacy_bios/boot.bin kernel/kernel.bin apps/apps.img doc clean
