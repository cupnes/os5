#include <efi.h>
#include <common.h>
#include <mem.h>

#define STACK_HEAP_SIZE	1048576	/* 1MB */

void efi_main(void *ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
	extern unsigned char _binary_kernel_bin_start;
	extern unsigned char _binary_kernel_bin_size;
	extern unsigned char _binary_apps_img_start;
	extern unsigned char _binary_apps_img_size;

	unsigned long long alloc_size;
	struct EFI_MEMORY_DESCRIPTOR *mdesc;
	unsigned long long alloc_addr;
	void *kernel_start;
	void *apps_start;
	unsigned char *p;
	unsigned int i;

	efi_init(SystemTable);

	puts(L"Starting OS5 UEFI bootloader ...\r\n");

	alloc_size = (unsigned long long)&_binary_kernel_bin_size +
		(unsigned long long)&_binary_apps_img_size + STACK_HEAP_SIZE;
	puts(L"(kern img size) + (apps img size) + (stack and heap size) = 0x");
	puth(alloc_size, 16);
	puts(L"\r\n");

	init_memmap();

	mdesc = get_allocatable_area(alloc_size);
	alloc_addr = mdesc->PhysicalStart;
	puts(L"alloc addr = 0x");
	puth(alloc_addr, 16);
	puts(L"\r\n");

	kernel_start = (void *)alloc_addr;
	ST->BootServices->CopyMem(kernel_start,
				  (void *)&_binary_kernel_bin_start,
				  (unsigned long long)&_binary_kernel_bin_size);
	alloc_addr += (unsigned long long)&_binary_kernel_bin_size;
	apps_start = (void *)alloc_addr;
	ST->BootServices->CopyMem(apps_start,
				  (void *)&_binary_apps_img_start,
				  (unsigned long long)&_binary_apps_img_size);

	puts(L"loaded kernel(first 8 bytes): ");
	p = (unsigned char *)kernel_start;
	for (i = 0; i < 8; i++) {
		puth(*p++, 2);
		putc(L' ');
	}
	puts(L"\r\n");
	puts(L"loaded apps(first 8 bytes): ");
	p = (unsigned char *)apps_start;
	for (i = 0; i < 8; i++) {
		puth(*p++, 2);
		putc(L' ');
	}
	puts(L"\r\n");

	exit_boot_services(ImageHandle);

	while (TRUE);
}
