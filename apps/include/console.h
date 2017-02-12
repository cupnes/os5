#ifndef _CONSOLE_H_
#define _CONSOLE_H_

unsigned int get_cursor_pos_y(void);
void put_str(char *str);
void put_str_pos(char *str, unsigned char x, unsigned char y);
void dump_hex(unsigned int val, unsigned int num_digits);
void dump_hex_pos(unsigned int val, unsigned int num_digits,
		  unsigned char x, unsigned char y);
unsigned int get_line(char *buf, unsigned int buf_size);

#endif /* _CONSOLE_H_ */
