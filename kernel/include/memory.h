#ifndef __MEMORY_H__
#define __MEMORY_H__

#define PAGE_SIZE	0x1000
#define PAGE_ADDR_MASK	0xfffff000

struct page_directory_entry {
	union {
		struct {
			unsigned int all;
		};
		struct {
			unsigned int p: 1, r_w: 1, u_s: 1, pwt: 1, pcd: 1, a: 1, reserved: 1, ps: 1, g: 1, usable: 3, pt_base: 20;
		};
	};
};
struct page_table_entry {
	union {
		struct {
			unsigned int all;
		};
		struct {
			unsigned int p: 1, r_w: 1, u_s: 1, pwt: 1, pcd: 1, a: 1, d: 1, pat: 1, g: 1, usable: 3, page_base: 20;
		};
	};
};

void mem_init(void);
void mem_page_start(void);
void *mem_alloc(void);
void mem_free(void *page);

#endif /* __MEMORY_H__ */
