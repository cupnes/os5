#define SCREEN_START				0xb8000
#define COLUMNS						80
#define ATTR						0x07
#define CHATT_CNT					1

void move_cursor_rel(unsigned int x, unsigned int y);
void put_char_pos(char c, unsigned char x, unsigned char y);
void put_char(char c);
void put_str(char *str);
void dump_hex(unsigned int val, unsigned int num_digits);
unsigned char get_keydata(void);
unsigned char get_keycode(void);
unsigned char get_keycode_pressed(void);
unsigned char get_keycode_released(void);
char get_char(void);
