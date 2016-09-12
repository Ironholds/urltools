#ifndef UTF8_H
#define UTF8_H

extern int locale_is_utf8;

/* is c the start of a utf8 sequence? */
#define isutf(c) (((c)&0xC0)!=0x80)

#define UEOF ((uint32_t)-1)

/* convert UTF-8 data to wide character */
size_t u8_toucs(uint32_t *dest, size_t sz, const char *src, size_t srcsz);

/* the opposite conversion */
size_t u8_toutf8(char *dest, size_t sz, const uint32_t *src, size_t srcsz);

#endif
