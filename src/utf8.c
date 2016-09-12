/*
  Basic UTF-8 manipulation routines
  by Jeff Bezanson
  placed in the public domain Fall 2005

  This code is designed to provide the utilities you need to manipulate
  UTF-8 as an internal string encoding. These functions do not perform the
  error checking normally needed when handling UTF-8 data, so if you happen
  to be from the Unicode Consortium you will want to flay me alive.
  I do this because error checking can be performed at the boundaries (I/O),
  with these routines reserved for higher performance on data known to be
  valid.
  A UTF-8 validation routine is included.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>
#include <wctype.h>

#ifdef WIN32
#include <malloc.h>
#define snprintf _snprintf
#else
#ifndef __FreeBSD__
#include <alloca.h>
#endif /* __FreeBSD__ */
#endif
#include <assert.h>

#include "utf8.h"

static const uint32_t offsetsFromUTF8[6] = {
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/* returns length of next utf-8 sequence */
size_t u8_seqlen(const char *s)
{
    return trailingBytesForUTF8[(unsigned int)(unsigned char)s[0]] + 1;
}

/* returns the # of bytes needed to encode a certain character
   0 means the character cannot (or should not) be encoded. */
size_t u8_charlen(uint32_t ch)
{
    if (ch < 0x80)
        return 1;
    else if (ch < 0x800)
        return 2;
    else if (ch < 0x10000)
        return 3;
    else if (ch < 0x110000)
        return 4;
    return 0;
}

size_t u8_codingsize(uint32_t *wcstr, size_t n)
{
    size_t i, c=0;

    for(i=0; i < n; i++)
        c += u8_charlen(wcstr[i]);
    return c;
}

/* conversions without error checking
   only works for valid UTF-8, i.e. no 5- or 6-byte sequences
   srcsz = source size in bytes
   sz = dest size in # of wide characters

   returns # characters converted
   if sz == srcsz+1 (i.e. 4*srcsz+4 bytes), there will always be enough space.
*/
size_t u8_toucs(uint32_t *dest, size_t sz, const char *src, size_t srcsz)
{
    uint32_t ch;
    const char *src_end = src + srcsz;
    size_t nb;
    size_t i=0;

    if (sz == 0 || srcsz == 0)
        return 0;

    while (i < sz) {
        if (!isutf(*src)) {     // invalid sequence
            dest[i++] = 0xFFFD;
            src++;
            if (src >= src_end) break;
            continue;
        }
        nb = trailingBytesForUTF8[(unsigned char)*src];
        if (src + nb >= src_end)
            break;
        ch = 0;
        switch (nb) {
            /* these fall through deliberately */
        case 5: ch += (unsigned char)*src++; ch <<= 6;
        case 4: ch += (unsigned char)*src++; ch <<= 6;
        case 3: ch += (unsigned char)*src++; ch <<= 6;
        case 2: ch += (unsigned char)*src++; ch <<= 6;
        case 1: ch += (unsigned char)*src++; ch <<= 6;
        case 0: ch += (unsigned char)*src++;
        }
        ch -= offsetsFromUTF8[nb];
        dest[i++] = ch;
    }
    return i;
}



/* srcsz = number of source characters
   sz = size of dest buffer in bytes

   returns # bytes stored in dest
   the destination string will never be bigger than the source string.
*/
size_t u8_toutf8(char *dest, size_t sz, const uint32_t *src, size_t srcsz)
{
    uint32_t ch;
    size_t i = 0;
    char *dest0 = dest;
    char *dest_end = dest + sz;

    while (i < srcsz) {
        ch = src[i];
        if (ch < 0x80) {
            if (dest >= dest_end)
                break;
            *dest++ = (char)ch;
        }
        else if (ch < 0x800) {
            if (dest >= dest_end-1)
                break;
            *dest++ = (ch>>6) | 0xC0;
            *dest++ = (ch & 0x3F) | 0x80;
        }
        else if (ch < 0x10000) {
            if (dest >= dest_end-2)
                break;
            *dest++ = (ch>>12) | 0xE0;
            *dest++ = ((ch>>6) & 0x3F) | 0x80;
            *dest++ = (ch & 0x3F) | 0x80;
        }
        else if (ch < 0x110000) {
            if (dest >= dest_end-3)
                break;
            *dest++ = (ch>>18) | 0xF0;
            *dest++ = ((ch>>12) & 0x3F) | 0x80;
            *dest++ = ((ch>>6) & 0x3F) | 0x80;
            *dest++ = (ch & 0x3F) | 0x80;
        }
        i++;
    }
    return (dest-dest0);
}

