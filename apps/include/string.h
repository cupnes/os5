#ifndef _STRING_H_
#define _STRING_H_

int str_get_len(const char *src);
int str_find_char(const char *src, char key);
void str_get_first_entry(const char *line, char *first, char *other);
int str_conv_ahex_int(const char *hex_str);
int str_compare(const char *src, const char *dst);

#endif /* _STRING_H_ */
