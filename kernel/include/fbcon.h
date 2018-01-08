#ifndef _FBCON_H_
#define _FBCON_H_

void fbcon_init(void);
void putc(char c);
void puts(char *s);
void puth(unsigned long long val, unsigned char num_digits);
char getc(void);

#endif
