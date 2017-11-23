all: fd.img

fs: boot/uefi/boot.efi kernel/kernel_64.bin apps/apps.img
	mkdir -p $@/EFI/BOOT
	cp boot/uefi/boot.efi $@/EFI/BOOT/BOOTX64.EFI
	cp kernel/kernel_64.bin $@/kernel.bin
	cp apps/apps.img $@/apps.img

fd.img: boot/legacy_bios/boot.bin kernel/kernel.bin apps/apps.img
	cat $+ > $@

boot/uefi/boot.efi:
	make -C boot/uefi

boot/legacy_bios/boot.bin:
	make -C boot/legacy_bios

kernel/kernel_64.bin:
	make -C kernel kernel_64.bin x86_64=true

kernel/kernel.bin:
	make -C kernel kernel.bin

apps/apps.img:
	make -C apps

doc:
	make -C doc

clean:
	make -C boot/uefi clean
	make -C boot/legacy_bios clean
	make -C kernel clean
	make -C apps clean
	make -C doc clean
	rm -rf *~ *.o *.bin *.dat *.img *.map fs

run: run32

run64: fs/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -bios OVMF.fd -hda fat:fs

run32: fd.img
	qemu-system-i386 -fda $<

.PHONY: boot/legacy_bios/boot.bin boot/uefi/boot.efi kernel/kernel.bin \
	apps/apps.img doc clean run run64 run32
