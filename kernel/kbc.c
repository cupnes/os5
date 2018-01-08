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

unsigned long long tmp_rsp;

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
}

unsigned char get_keydata_noir(void)
{
	while (!(inb_p(IOADR_KBC_STATUS) & IOADR_KBC_STATUS_BIT_OBF));
	return inb_p(IOADR_KBC_DATA);
}

unsigned char get_keydata(void)
{
	unsigned char data;
	unsigned char dequeuing = 1;
	unsigned char if_bit;

	while (dequeuing) {
		kern_lock(&if_bit);
		data = dequeue(&keycode_queue);
		if (!error_status)
			dequeuing = 0;
		kern_unlock(&if_bit);
	}

	__asm__ ("movq	%%rbp, %[tmp_rsp]":[tmp_rsp]"=r"(tmp_rsp):);
	tmp_rsp += 8;
	return data;
}

unsigned char get_keycode_pressed(void)
{
	unsigned char keycode;
	while (1) {
		keycode = get_keydata();

		putc('#');
		puth(tmp_rsp, 16);
		puts("\r\n");

		if (!(keycode & IOADR_KBC_DATA_BIT_BRAKE))
			break;
	}
	return keycode & ~IOADR_KBC_DATA_BIT_BRAKE;
}
