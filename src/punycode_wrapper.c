#include <R.h>
#include <Rinternals.h>

#include <stdint.h>
#include <stdlib.h>

#include "punycode.h"
#include "utf8.h"

#define CHARPT(x,i) (CHAR(STRING_ELT(x,i)))

// Storage buffers
#define BUFLEN 2048
static char buf[BUFLEN];
static uint32_t ibuf[BUFLEN];
static char* ascii = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ";

#define CHECKINPUT(x) if (TYPEOF(x) != STRSXP) \
  error("input must be a string\n");

#define CHECKLEN(s,slen) if (slen > BUFLEN) \
  error("input string=%s too big; must have no more than %d chars\n", s, BUFLEN)

static inline char* punycode_errcode_lookup(enum punycode_status st)
{
  char *ret;
  if (st == punycode_bad_input)
    ret = "input is invalid";
  else if (st == punycode_big_output)
    ret = "output would exceed the space provided";
  else if (st == punycode_overflow)
    ret = "input needs wider integers to process";
  else
  {
    ret = (char*) R_alloc(30, 1);
    sprintf(ret, "unknown error with code %d", st);
  }
  
  return ret;
}

static inline void clearbuf()
{
  for (int i=0; i<BUFLEN; i++)
  {
    buf[i] = '\0';
    ibuf[i] = 0;
  }
}



SEXP topuny(SEXP s_){
  SEXP ret;
  const int len = LENGTH(s_);
  
  CHECKINPUT(s_);
  PROTECT(ret = allocVector(STRSXP, len));
  
  for (int i=0; i<len; i++) {
    
    if(i % 10000 == 0){
      R_CheckUserInterrupt();
    }
    if(STRING_ELT(s_, i) == NA_STRING){
      SET_STRING_ELT(ret, i, NA_STRING);
    } else {
      punycode_uint buflen = BUFLEN;
      punycode_uint unilen = BUFLEN;
      const char *s = CHARPT(s_, i);
      const int slen = strlen(s);
      
      CHECKLEN(s, slen);
      
      if(strspn(s, ascii) != slen){
        unilen = u8_toucs(ibuf, unilen, s, slen);
        
        enum punycode_status st = punycode_encode(unilen, ibuf, NULL, &buflen, buf);
        
        if(st != punycode_success){
          warning("Error in converting element %d: %s\n", i+1,
                  punycode_errcode_lookup(st));
          SET_STRING_ELT(ret, i, NA_STRING);
        } else {
          SET_STRING_ELT(ret, i, mkCharLen(buf, buflen));
        }

      } else {
        SET_STRING_ELT(ret, i, STRING_ELT(s_, i));
      }
;
    }
  }
  
  clearbuf();
  
  UNPROTECT(1);
  return ret;
}



SEXP unpuny(SEXP s_)
{
  SEXP ret;
  const int len = LENGTH(s_);
  
  CHECKINPUT(s_);
  
  PROTECT(ret = allocVector(STRSXP, len));
  
  for (int i = 0; i < len; i++) {
    
    if(i % 10000 == 0){
      R_CheckUserInterrupt();
    }
    
    if(STRING_ELT(s_, i) == NA_STRING){
      SET_STRING_ELT(ret, i, NA_STRING);
    } else {
      punycode_uint buflen;
      punycode_uint unilen = BUFLEN;
      const char *s = CHARPT(s_, i);
      const int slen = strlen(s);
      
      CHECKLEN(s, slen);
      
      enum punycode_status st = punycode_decode(slen, s, &unilen, ibuf, NULL);
      
      if(st != punycode_success){
        warning("Error in converting element %d: %s\n", i+1,
                punycode_errcode_lookup(st));
        SET_STRING_ELT(ret, i, NA_STRING);
      } else {
        buflen = u8_toutf8(buf, BUFLEN, ibuf, unilen);
        SET_STRING_ELT(ret, i, mkCharLenCE(buf, buflen, CE_UTF8));
      }

    }
  }
  
  clearbuf();
  
  UNPROTECT(1);
  return ret;
}

