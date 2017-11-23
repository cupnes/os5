#include <efi.h>
#include <common.h>
#include <file.h>

struct FILE file_list[MAX_FILE_NUM];

unsigned long long get_file_size(struct EFI_FILE_PROTOCOL *file)
{
	unsigned long long status;
	unsigned long long fi_size = FILE_INFO_BUF_SIZE;
	unsigned long long fi_buf[FILE_INFO_BUF_SIZE];
	struct EFI_FILE_INFO *fi_ptr;

	status = file->GetInfo(file, &fi_guid, &fi_size, fi_buf);
	if (!check_warn_error(status, L"file->GetInfo failure."))
		return 0;

	fi_ptr = (struct EFI_FILE_INFO *)fi_buf;

	return fi_ptr->FileSize;
}
