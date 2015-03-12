int main(void)
{
	volatile int foo;

	*(volatile unsigned short *)0x000a8000 = (unsigned short)0x0741;

	while (1) {
		foo = 0xbeefcafe;
	}

	return 0;
}
