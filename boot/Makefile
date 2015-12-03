.s.o:
	as -o $@ $<

boot.bin: boot.o
	ld -o $@ $< -T boot.ld -Map boot.map

boot.o: boot.s

clean:
	rm -f *~ *.o *.bin *.dat *.img *.map

.PHONY: clean
