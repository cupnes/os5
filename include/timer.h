#ifndef _TIMER_H_
#define _TIMER_H_

#define IOADR_PIT_COUNTER0	0x0040
#define IOADR_PIT_CONTROL_WORD	0x0043
#define IOADR_PIT_CONTROL_WORD_BIT_COUNTER0		0x00
#define IOADR_PIT_CONTROL_WORD_BIT_16BIT_READ_LOAD	0x30
#define IOADR_PIT_CONTROL_WORD_BIT_MODE2		0x04	/* Rate Generator */

#define INTR_IR_TIMER		0
#define INTR_NUM_TIMER		32
#define INTR_MASK_BIT_TIMER	0x01

#define TIMER_TICK_MS		10

extern unsigned char timer_handler;
extern unsigned int global_counter;

void timer_init(void);

#endif /* _TIMER_H_ */
