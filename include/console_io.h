#ifndef _CONSOLE_IO_H_
#define _CONSOLE_IO_H_

#define IOADR_KBC_DATA				0x0060
#define IOADR_KBC_DATA_BIT_BRAKE	0x80
#define IOADR_KBC_STATUS			0x0064
#define IOADR_KBC_STATUS_BIT_OBF	0x01

#define INTR_IR_KB					1
#define INTR_NUM_KB					33
#define INTR_MASK_BIT_KB			0x02
#define SCREEN_START				0xb8000
#define COLUMNS						80
#define ROWS						25
#define ATTR						0x07
#define CHATT_CNT					1

#define ASCII_ESC					0x1b
#define ASCII_BS					0x08
#define ASCII_HT					0x09

#define EVENT_TYPE_KBD			1

struct cursor_position {
	unsigned int x, y;
};

extern unsigned char keyboard_handler;
extern struct cursor_position cursor_pos;

void con_init(void);
void update_cursor(void);
void put_char_pos(char c, unsigned char x, unsigned char y);
void put_char(char c);
void put_str(char *str);
void put_str_pos(char *str, unsigned char x, unsigned char y);
void dump_hex(unsigned int val, unsigned int num_digits);
void dump_hex_pos(unsigned int val, unsigned int num_digits, unsigned char x, unsigned char y);
unsigned char get_keydata_noir(void);
unsigned char get_keydata(void);
unsigned char get_keycode(void);
unsigned char get_keycode_pressed(void);
unsigned char get_keycode_released(void);
char get_char(void);
unsigned int get_line(char *buf, unsigned int buf_size);

#endif /* _CONSOLE_IO_H_ */
