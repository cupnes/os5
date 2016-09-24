NAME=libcommon

$(LIB_DIR)/$(NAME).a: $(NAME).o
	ar rcs $@ $<

%.o: %.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf *~ *.o *.a *.map

.PHONY: clean
