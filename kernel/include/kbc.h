#ifndef _KBC_H_
#define _KBC_H_

#define IOADR_KBC_DATA	0x0060
#define IOADR_KBC_DATA_BIT_BRAKE	0x80
#define IOADR_KBC_STATUS	0x0064
#define IOADR_KBC_STATUS_BIT_OBF	0x01

#define ASCII_ESC	0x1b
#define ASCII_BS	0x08
#define ASCII_HT	0x09

extern const char keymap[];

unsigned char get_keycode_pressed(void);

#endif
