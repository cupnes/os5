#ifndef _FS_H_
#define _FS_H_

#include <list.h>

#define MAX_FILE_NAME			32
#define RESERVED_FILE_HEADER_SIZE	15

struct file_head {
	struct list lst;
	unsigned char num_files;
};

struct file_header {
	char name[MAX_FILE_NAME];
	unsigned char block_num;
	unsigned char reserve[RESERVED_FILE_HEADER_SIZE];
};

struct file {
	struct list lst;
	struct file_header *head;
	void *data_base_addr;
};

extern struct file *fshell;

void fs_init(void *fs_base_addr);
struct file *fs_open(const char *name);
int fs_close(struct file *f);

#endif /* _FS_H_ */
