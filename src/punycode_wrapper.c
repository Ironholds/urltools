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



SEXP make_puny(SEXP s_)
{
  SEXP ret;
  size_t nconv;
  size_t buflen;
  const int len = LENGTH(s_);

  if (TYPEOF(s_) != STRSXP)
    error("strings only\n");

  PROTECT(ret = allocVector(STRSXP, len));

  for (int i=0; i<len; i++)
  {
    buflen = BUFLEN;
    const char *s = CHARPT(s_, i);

    /*mbstowcs(ibuf, s, strlen(s));*/
    size_t unilen = u8_toucs(ibuf, BUFLEN, s, strlen(s));
    
    // FIXME nconv is number of conversions; if that info isn't useful, delete the var; if it is, return a list or something
    nconv = punycode_encode(ibuf, unilen, buf, &buflen);

    SET_STRING_ELT(ret, i, mkCharLen(buf, buflen));
  }

  clearbuf();

  UNPROTECT(1);
  return ret;
}

