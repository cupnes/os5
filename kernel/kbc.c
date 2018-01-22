#include <io_port.h>
#include <intr.h>
#include <kbc.h>
#include <lock.h>
#include <common.h>

#include <cpu.h>
#include <fbcon.h>

const char keymap[] = {
	0x00, ASCII_ESC, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '^', ASCII_BS, ASCII_HT,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '@', '[', '\n', 0x00, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	':', 0x00, 0x00, ']', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0x00, '*',
	0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, '_', 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, '\\', 0x00, 0x00
};

struct queue keycode_queue;

unsigned long long tmp_rbp_b;
unsigned long long tmp_rbp_1;
unsigned long long tmp_rbp_2;
unsigned long long tmp_rbp_3;
unsigned long long tmp_rbp_4;
unsigned long long tmp_rbp_5;
unsigned long long tmp_rbp;

void do_ir_keyboard(void)
{
	unsigned char status, data;

	status = inb_p(IOADR_KBC_STATUS);
	if (status & IOADR_KBC_STATUS_BIT_OBF) {
		data = inb_p(IOADR_KBC_DATA);
		enqueue(&keycode_queue, data);
	}
	outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_KB,
	       IOADR_MPIC_OCW2);
}

void kbc_init(void)
{
	keycode_queue.start = 0;
	keycode_queue.end = 0;
	keycode_queue.is_full = 0;
	error_status = 0;

	tmp_rbp_b = 0;
}

unsigned char get_keydata_noir(void)
{
	while (!(inb_p(IOADR_KBC_STATUS) & IOADR_KBC_STATUS_BIT_OBF));
	return inb_p(IOADR_KBC_DATA);
}

void assert_rbp(unsigned long long rbp, unsigned int id)
{
	if (rbp != tmp_rbp_b) {
		putc('E');
		puth(id, 2);
		putc(' ');
		puth(rbp, 16);
		putc(',');
		puth(tmp_rbp_b, 16);
		while (1);
	}
}

unsigned char get_keydata(void)
{
	unsigned char data;
	unsigned char dequeuing = 1;
	unsigned char if_bit;

	__asm__ ("movq	%%rbp, %[tmp_rbp_b]":[tmp_rbp_b]"=r"(tmp_rbp_b):);

	while (dequeuing) {
		__asm__ ("movq	%%rbp, %[tmp_rbp]":[tmp_rbp]"=r"(tmp_rbp):);
		assert_rbp(tmp_rbp, 1);
		kern_lock(&if_bit);
		__asm__ ("movq	%%rbp, %[tmp_rbp]":[tmp_rbp]"=r"(tmp_rbp):);
		assert_rbp(tmp_rbp, 2);
		data = dequeue(&keycode_queue);
		__asm__ ("movq	%%rbp, %[tmp_rbp]":[tmp_rbp]"=r"(tmp_rbp):);
		assert_rbp(tmp_rbp, 3);
		if (!error_status)
			dequeuing = 0;
		__asm__ ("movq	%%rbp, %[tmp_rbp]":[tmp_rbp]"=r"(tmp_rbp):);
		assert_rbp(tmp_rbp, 4);
		kern_unlock(&if_bit);
		__asm__ ("movq	%%rbp, %[tmp_rbp]":[tmp_rbp]"=r"(tmp_rbp):);
		assert_rbp(tmp_rbp, 5);
	}

	__asm__ ("movq	%%rbp, %[tmp_rbp]":[tmp_rbp]"=r"(tmp_rbp):);
	assert_rbp(tmp_rbp, 6);
	return data;
}

unsigned char get_keycode_pressed(void)
{
	unsigned char keycode;
	while (1) {
		keycode = get_keydata();

		putc('#');
		puth(tmp_rbp_b, 16);
		putc(',');
		puth(tmp_rbp, 16);
		puts("\r\n");

		if (!(keycode & IOADR_KBC_DATA_BIT_BRAKE))
			break;
	}
	return keycode & ~IOADR_KBC_DATA_BIT_BRAKE;
}
