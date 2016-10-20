#include <app.h>
#include <kernel.h>
#include <console.h>

#define MAX_LINE_SIZE	512

int main(int argc, char *argv[])
{
	volatile unsigned char *p;
	unsigned int addr;
	unsigned char c = 0;
	char buf[MAX_LINE_SIZE];

	while (1) {
		for (addr = 0x203f0000; addr < 0x20400000; addr++) {
			p = (volatile unsigned char *)addr;
			*p = c;
		}
		get_line(buf, MAX_LINE_SIZE);
		c++;
	}

	return 0;
}
