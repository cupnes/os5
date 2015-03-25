#define IOADR_MPIC_OCW1				0x0021
#define IOADR_KBC_DATA				0x0060
#define IOADR_KBC_DATA_BIT_BRAKE	0x80
#define IOADR_KBC_STATUS			0x0064
#define IOADR_KBC_STATUS_BIT_OBF	0x01
#define IOADR_SPIC_OCW1				0x00a1

#define outb(value, port) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))


#define inb(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})
