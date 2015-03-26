#define IOADR_MPIC_ICW1	0x0020
#define IOADR_MPIC_ICW2	0x0021
#define IOADR_MPIC_ICW3	0x0021
#define IOADR_MPIC_ICW4	0x0021
#define IOADR_MPIC_OCW1	0x0021
#define IOADR_SPIC_ICW1	0x00a0
#define IOADR_SPIC_ICW2	0x00a1
#define IOADR_SPIC_ICW3	0x00a1
#define IOADR_SPIC_ICW4	0x00a1
#define IOADR_SPIC_OCW1	0x00a1

void intr_init(void);
void intr_set_mask_master(unsigned char mask);
unsigned char intr_get_mask_master(void);
void intr_set_mask_slave(unsigned char mask);
unsigned char intr_get_mask_slave(void);
