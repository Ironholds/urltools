#include <R.h>
#include <Rinternals.h>

#include <stdint.h>
#include <stdlib.h>

#include "punycode.h"
#include "utf8.h"

#define CHARPT(x,i) (CHAR(STRING_ELT(x,i)))



#define BUFLEN 1024
char buf[BUFLEN];
uint32_t ibuf[BUFLEN];

static inline void clearbuf()
{
  for (int i=0; i<BUFLEN; i++)
  {
    buf[i] = '\0';
    ibuf[i] = 0;
  }
}



SEXP topuny(SEXP s_)
{
  SEXP ret;
  const int len = LENGTH(s_);

  if (TYPEOF(s_) != STRSXP)
    error("strings only\n");

  PROTECT(ret = allocVector(STRSXP, len));

  for (int i=0; i<len; i++)
  {
    size_t buflen = BUFLEN;
    size_t unilen = BUFLEN;
    const char *s = CHARPT(s_, i);

    const int slen = strlen(s);
    if (slen > BUFLEN)
      error("string too big");

    /*unilen = mbstowcs(ibuf, s, slen);*/
    unilen = u8_toucs(ibuf, unilen, s, slen);
    
    punycode_encode(ibuf, unilen, buf, &buflen);

    SET_STRING_ELT(ret, i, mkCharLen(buf, buflen));
  }

  clearbuf();

  UNPROTECT(1);
  return ret;
}



SEXP unpuny(SEXP s_)
{
  SEXP ret;
  const int len = LENGTH(s_);

  if (TYPEOF(s_) != STRSXP)
    error("strings only\n");

  PROTECT(ret = allocVector(STRSXP, len));

  for (int i=0; i<len; i++)
  {
    size_t buflen;
    size_t unilen = BUFLEN;
    const char *s = CHARPT(s_, i);

    const int slen = strlen(s);
    if (slen > BUFLEN)
      error("string too big");

    punycode_decode(s, slen, ibuf, &unilen);

    /*buflen = wcstombs(buf, ibuf, unilen*sizeof(uint32_t));*/
    buflen = u8_toutf8(buf, BUFLEN, ibuf, unilen);

    SET_STRING_ELT(ret, i, mkCharLen(buf, buflen));
    clearbuf();
  }

  clearbuf();

  UNPROTECT(1);
  return ret;
}
