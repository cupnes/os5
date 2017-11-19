.s.o:
	as --32 -o $@ $<

boot.bin: boot.o
	ld -m elf_i386 -o $@ $< -T boot.ld -Map boot.map

boot.o: boot.s

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

.PHONY: clean
