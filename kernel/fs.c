#include <fs.h>
#include <stddef.h>
#include <memory.h>
#include <list.h>
#include <queue.h>
#include <common.h>

struct file_head fhead;
struct file *fshell;

void fs_init(void *fs_base_addr)
{
	struct file *f;
	unsigned char i;
	unsigned char *file_start_addr = fs_base_addr;

	queue_init((struct list *)&fhead);
	fhead.num_files = *(unsigned char *)fs_base_addr;

	file_start_addr += PAGE_SIZE;
	for (i = 1; i <= fhead.num_files; i++) {
		f = (struct file *)mem_alloc();
		f->head = (struct file_header *)file_start_addr;
		f->data_base_addr =
			(char *)file_start_addr + sizeof(struct file_header);
		file_start_addr += PAGE_SIZE * f->head->block_num;
		queue_enq((struct list *)f, (struct list *)&fhead);
	}
	fshell = (struct file *)fhead.lst.next;
}

struct file *fs_open(const char *name)
{
	struct file *f;

	/* 将来的には、struct fileのtask_idメンバにopenしたタスクの
	 * TASK_IDを入れるようにする。そして、openしようとしているファ
	 * イルのtask_idが既に設定されていれば、fs_openはエラーを返す
	 * ようにする */

	for (f = (struct file *)fhead.lst.next; f != (struct file *)&fhead;
	     f = (struct file *)f->lst.next) {
		if (!str_compare(name, f->head->name))
			return f;
	}

	return NULL;
}

int fs_close(struct file *f __attribute__ ((unused)))
{
	/* 将来的には、fidに対応するstruct fileのtask_idメンバーを設定
	 * なし(0)にする。 */
	return 0;
}
