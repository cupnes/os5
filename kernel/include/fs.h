#ifndef _FS_H_
#define _FS_H_

#include <list.h>

struct file_head {
	struct list lst;
	unsigned char num_files;
};

struct file {
	struct list lst;
	char *name;
	void *data_base_addr;
};

extern struct file *fshell;

void fs_init(void *fs_base_addr);
struct file *fs_open(const char *name);
int fs_close(struct file *f);

#endif /* _FS_H_ */
