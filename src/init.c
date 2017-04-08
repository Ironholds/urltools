#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP urltools_finalise_suffixes(SEXP, SEXP, SEXP, SEXP);
extern SEXP urltools_get_component_(SEXP, SEXP);
extern SEXP urltools_get_credentials(SEXP);
extern SEXP urltools_host_extract_(SEXP);
extern SEXP urltools_param_get(SEXP, SEXP);
extern SEXP urltools_param_remove(SEXP, SEXP);
extern SEXP urltools_param_set(SEXP, SEXP, SEXP);
extern SEXP urltools_puny_decode(SEXP);
extern SEXP urltools_puny_encode(SEXP);
extern SEXP urltools_reverse_strings(SEXP);
extern SEXP urltools_set_component_(SEXP, SEXP, SEXP);
extern SEXP urltools_strip_credentials(SEXP);
extern SEXP urltools_tld_extract_(SEXP);
extern SEXP urltools_url_compose(SEXP);
extern SEXP urltools_url_decode(SEXP);
extern SEXP urltools_url_encode(SEXP);
extern SEXP urltools_url_parse(SEXP);

static const R_CallMethodDef CallEntries[] = {
  {"urltools_finalise_suffixes", (DL_FUNC) &urltools_finalise_suffixes, 4},
  {"urltools_get_component_",    (DL_FUNC) &urltools_get_component_,    2},
  {"urltools_get_credentials",   (DL_FUNC) &urltools_get_credentials,   1},
  {"urltools_host_extract_",     (DL_FUNC) &urltools_host_extract_,     1},
  {"urltools_param_get",         (DL_FUNC) &urltools_param_get,         2},
  {"urltools_param_remove",      (DL_FUNC) &urltools_param_remove,      2},
  {"urltools_param_set",         (DL_FUNC) &urltools_param_set,         3},
  {"urltools_puny_decode",       (DL_FUNC) &urltools_puny_decode,       1},
  {"urltools_puny_encode",       (DL_FUNC) &urltools_puny_encode,       1},
  {"urltools_reverse_strings",   (DL_FUNC) &urltools_reverse_strings,   1},
  {"urltools_set_component_",    (DL_FUNC) &urltools_set_component_,    3},
  {"urltools_strip_credentials", (DL_FUNC) &urltools_strip_credentials, 1},
  {"urltools_tld_extract_",      (DL_FUNC) &urltools_tld_extract_,      1},
  {"urltools_url_compose",       (DL_FUNC) &urltools_url_compose,       1},
  {"urltools_url_decode",        (DL_FUNC) &urltools_url_decode,        1},
  {"urltools_url_encode",        (DL_FUNC) &urltools_url_encode,        1},
  {"urltools_url_parse",         (DL_FUNC) &urltools_url_parse,         1},
  {NULL, NULL, 0}
};

void R_init_urltools(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}