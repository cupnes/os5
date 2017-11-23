#include <efi.h>
#include <common.h>
#include <mem.h>
#include <fb.h>
#include <file.h>

#define KERNEL_FILE_NAME	L"kernel.bin"
#define APPS_FILE_NAME	L"apps.img"
#define STACK_HEAP_SIZE	1048576	/* 1MB */

void efi_main(void *ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
	unsigned long long status;
	struct EFI_FILE_PROTOCOL *root;
	struct EFI_FILE_PROTOCOL *file_kernel;
	struct EFI_FILE_PROTOCOL *file_apps;
	unsigned long long kernel_size;
	unsigned long long apps_size;
	unsigned long long alloc_size;
	struct EFI_MEMORY_DESCRIPTOR *mdesc;
	unsigned long long alloc_addr;
	unsigned long long stack_base;
	void *kernel_start;
	void *apps_start;
	unsigned char *p;
	unsigned int i;
	unsigned long long kernel_arg1;
	unsigned long long kernel_arg2;
	unsigned char has_apps = TRUE;

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
	if (!check_warn_error(status, L"root->Open(apps)")) {
		puts(L"apps load failure. skip.\r\n");
		has_apps = FALSE;
	}

	if (has_apps)
		apps_size = get_file_size(file_apps);

	alloc_size = kernel_size + STACK_HEAP_SIZE;
	if (has_apps) {
		puts(L"(kern bin size) + (apps img size) + ");
		puts(L"(stack and heap size) = 0x");
		alloc_size += apps_size;
	} else
		puts(L"(kern bin size) + (stack and heap size) = 0x");

	puth(alloc_size, 16);
	puts(L"\r\n");

	init_memmap();

	mdesc = get_allocatable_area(alloc_size);
	alloc_addr = mdesc->PhysicalStart;
	puts(L"alloc addr = 0x");
	puth(alloc_addr, 16);
	puts(L"\r\n");
	stack_base = mdesc->PhysicalStart + (mdesc->NumberOfPages * PAGE_SIZE);
	puts(L"stack_base = 0x");
	puth(stack_base, 16);
	puts(L"\r\n");

	kernel_start = (void *)alloc_addr;
	status = file_kernel->Read(file_kernel, &kernel_size, kernel_start);
	assert(status, L"file_kernel->Read");

	if (has_apps) {
		alloc_addr += kernel_size;
		apps_start = (void *)alloc_addr;
		status = file_apps->Read(file_apps, &apps_size, apps_start);
		assert(status, L"file_apps->Read");
	}

	puts(L"loaded kernel(first 8 bytes): ");
	p = (unsigned char *)kernel_start;
	for (i = 0; i < 8; i++) {
		puth(*p++, 2);
		putc(L' ');
	}
	puts(L"\r\n");
	if (has_apps) {
		puts(L"loaded apps(first 8 bytes): ");
		p = (unsigned char *)apps_start;
		for (i = 0; i < 8; i++) {
			puth(*p++, 2);
			putc(L' ');
		}
		puts(L"\r\n");
	}

	file_kernel->Close(file_kernel);
	if (has_apps)
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
	puth(stack_base, 16);
	puts(L"\r\n");

	exit_boot_services(ImageHandle);

	__asm__ ("	mov	%0, %%rsi\n"
		 "	mov	%1, %%rdi\n"
		 "	mov	%2, %%rsp\n"
		 "	jmp	*%3\n"
		 ::"r"(kernel_arg2), "r"(kernel_arg1), "r"(stack_base),
		  "r"(kernel_start));

	while (TRUE);
}
