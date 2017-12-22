#include <efi.h>
#include <common.h>
#include <mem.h>
#include <fb.h>
#include <file.h>

#define KERNEL_FILE_NAME	L"kernel.bin"
#define APPS_FILE_NAME	L"apps.img"
#define STACK_HEAP_SIZE	1048576	/* 1MB */

#define KERNEL_START	0x0000000000110000
#define APPS_START	0x0000000000200000
#define STACK_BASE	0x0000000000400000

void efi_main(void *ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
	unsigned long long status;
	struct EFI_FILE_PROTOCOL *root;
	struct EFI_FILE_PROTOCOL *file_kernel;
	struct EFI_FILE_PROTOCOL *file_apps;
	unsigned long long kernel_size;
	unsigned long long apps_size;
	unsigned char *p;
	unsigned int i;
	unsigned long long kernel_arg1;
	unsigned long long kernel_arg2;

	efi_init(SystemTable);

	puts(L"Starting OS5 UEFI bootloader ...\r\n");

	status = SFSP->OpenVolume(SFSP, &root);
	assert(status, L"SFSP->OpenVolume");

	status = root->Open(
		root, &file_kernel, KERNEL_FILE_NAME, EFI_FILE_MODE_READ, 0);
	assert(status, L"root->Open(kernel)");

	kernel_size = get_file_size(file_kernel);

	status = root->Open(
		root, &file_apps, APPS_FILE_NAME, EFI_FILE_MODE_READ, 0);
	assert(status, L"root->Open(apps)");

	apps_size = get_file_size(file_apps);

	status = file_kernel->Read(file_kernel, &kernel_size,
				   (void *)KERNEL_START);
	assert(status, L"file_kernel->Read");
	status = file_apps->Read(file_apps, &apps_size, (void *)APPS_START);
	assert(status, L"file_apps->Read");

	puts(L"loaded kernel(first 8 bytes): ");
	p = (unsigned char *)KERNEL_START;
	for (i = 0; i < 8; i++) {
		puth(*p++, 2);
		putc(L' ');
	}
	puts(L"\r\n");
	puts(L"loaded apps(first 8 bytes): ");
	p = (unsigned char *)APPS_START;
	for (i = 0; i < 8; i++) {
		puth(*p++, 2);
		putc(L' ');
	}
	puts(L"\r\n");

	file_kernel->Close(file_kernel);
	file_apps->Close(file_apps);

	kernel_arg1 = (unsigned long long)ST;
	init_fb();
	kernel_arg2 = (unsigned long long)&fb;

	puts(L"kernel_arg1 = 0x");
	puth(kernel_arg1, 16);
	puts(L"\r\n");
	puts(L"kernel_arg2 = 0x");
	puth(kernel_arg2, 16);
	puts(L"\r\n");
	puts(L"stack_base = 0x");
	puth(STACK_BASE, 16);
	puts(L"\r\n");

	exit_boot_services(ImageHandle);

	unsigned long long _sb = STACK_BASE, _ks = KERNEL_START;
	__asm__ ("	mov	%0, %%rsi\n"
		 "	mov	%1, %%rdi\n"
		 "	mov	%2, %%rsp\n"
		 "	jmp	*%3\n"
		 ::"r"(kernel_arg2), "r"(kernel_arg1), "r"(_sb), "r"(_ks));

	while (TRUE);
}
