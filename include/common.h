#ifndef _COMMON_H_
#define _COMMON_H_

void copy_mem(const void *src, void *dst, unsigned int size);
int pow(int num, int multer);
int str_find_char(const char *src, char key);
int str_get_len(const char *src);
int str_compare(const char *src, const char *dst);

#endif /* _COMMON_H_ */
