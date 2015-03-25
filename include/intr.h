#define IOADR_MPIC_OCW1		0x0021
#define IOADR_SPIC_OCW1		0x00a1

void intr_set_mask_master(unsigned char mask);
unsigned char intr_get_mask_master(void);
void intr_set_mask_slave(unsigned char mask);
unsigned char intr_get_mask_slave(void);
