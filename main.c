int main(void)
{
	volatile int foo;

	while (1) {
		foo = 0xbeefcafe;
	}

	return 0;
}
