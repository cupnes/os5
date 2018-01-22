#ifndef _KBC_H_
#define _KBC_H_

#define IOADR_KBC_DATA	0x0060
#define IOADR_KBC_DATA_BIT_BRAKE	0x80
#define IOADR_KBC_STATUS	0x0064
#define IOADR_KBC_STATUS_BIT_OBF	0x01

#define ASCII_ESC	0x1b
#define ASCII_BS	0x08
#define ASCII_HT	0x09

#define INTR_IR_KB	1
#define INTR_NUM_KB	33
#define INTR_MASK_BIT_KB	0x02

extern const char keymap[];
extern unsigned char keyboard_handler;

extern unsigned long long tmp_rbp_b;
extern unsigned long long tmp_rbp_1;
extern unsigned long long tmp_rbp_2;
extern unsigned long long tmp_rbp_3;
extern unsigned long long tmp_rbp_4;
extern unsigned long long tmp_rbp_5;
extern unsigned long long tmp_rbp;

unsigned char get_keycode_pressed(void);
void kbc_init(void);

#endif
