#define PHY_BASE_ADDR	(0x3f000000)
#define AUX_BASE	(0x00215000)
#define AUX_ENABLES	((volatile unsigned int *)(PHY_BASE_ADDR + AUX_BASE + 0x04))
#define MU_BASE		(PHY_BASE_ADDR + 0x00215040)
#define MU_IER_RXFIFO_CLR	(1U << 1)
#define MU_IER_TXFIFO_CLR	(2U << 1)
#define MU_LSR_TX_IDLE	(1U << 6)
#define MU_LSR_TX_EMPTY	(1U << 5)
#define MU_LSR_RX_OVERRUN	(1U << 1)
#define MU_LSR_RX_RDY	(1U << 0)

struct bcm283x_mu_regs {
	unsigned int io;
	unsigned int ier;
	unsigned int iir;
	unsigned int lcr;
	unsigned int mcr;
	unsigned int lsr;
	unsigned int msr;
	unsigned int scratch;
	unsigned int cntl;
	unsigned int stat;
	unsigned int baud;
};

volatile struct bcm283x_mu_regs *mu = (struct bcm283x_mu_regs *)MU_BASE;

void put_char(char ch)
{
	while (!(mu->lsr && MU_LSR_TX_IDLE) && !(mu->lsr && MU_LSR_TX_EMPTY));
	mu->io = (unsigned int)ch;
}

void put_str(char *str)
{
	while (*str != '\0')
		put_char(*str++);
}

void dump_hex(unsigned int val, unsigned int num_digits)
{
	unsigned int tmp;

	for (; num_digits > 0; num_digits--) {
		tmp = val & (0xfU << ((num_digits - 1) * 4));
		tmp >>= (num_digits - 1) * 4;
		if (tmp < 0xa)
			put_char('0' + tmp);
		else
			put_char('A' + tmp - 0xa);
	}
}

char get_char(void)
{
	while (!(mu->lsr & MU_LSR_RX_RDY));
	return (char)mu->io;
}

void dump_mu_registers(void)
{
	struct bcm283x_mu_regs tmp;

	tmp.io = mu->io;
	tmp.ier = mu->ier;
	tmp.iir = mu->iir;
	tmp.lcr = mu->lcr;
	tmp.mcr = mu->mcr;
	tmp.lsr = mu->lsr;
	tmp.msr = mu->msr;
	tmp.scratch = mu->scratch;
	tmp.cntl = mu->cntl;
	tmp.stat = mu->stat;
	tmp.baud = mu->baud;

	put_str("io=");
	dump_hex(tmp.io, 2);
	put_str(", ier=");
	dump_hex(tmp.ier, 2);
	put_str(", iir=");
	dump_hex(tmp.iir, 2);
	put_str(", lcr=");
	dump_hex(tmp.lcr, 2);
	put_str(", mcr=");
	dump_hex(tmp.mcr, 2);
	put_str(", lsr=");
	dump_hex(tmp.lsr, 2);
	put_str(", msr=");
	dump_hex(tmp.msr, 2);
	put_str(", scratch=");
	dump_hex(tmp.scratch, 2);
	put_str(", cntl=");
	dump_hex(tmp.cntl, 2);
	put_str(", stat=");
	dump_hex(tmp.stat, 8);
	put_str(", baud=");
	dump_hex(tmp.baud, 4);
	put_str("\r\n");
}

int main(void)
{
	while (1)
		put_char(get_char());

	return 0;
}
