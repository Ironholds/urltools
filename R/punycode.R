#' Puncycode Encoder/Decoder
#' 
#' Encode/Decode a UTF-8 string to/from its punycode representation.  See references
#' for details.
#' 
#' @param s
#' A string or vector of strings.
#' 
#' @return
#' A string or vector of strings.
#' 
#' @references
#' Punycode RFC \url{http://www.faqs.org/rfcs/rfc3492.html}
#' Implementation \url{https://dxr.mozilla.org/mozilla-central/source/netwerk/dns/punycode.c}
#' 
#' @examples
#' \dontrun{
#' library(urltools)
#' str <- "bücher"
#' (p <- topuny(str))
#' ## [1] "bcher-kva"
#' 
#' unpuny(p)
#' ## [1] "bücher"
#' }
#' 
#' @name punycode
#' @rdname punycode
NULL

#' @rdname punycode
#' @export
puny_encode <- function(s)
{
  .Call("topuny", s, PACKAGE="urltools")
}

#' @rdname punycode
#' @export
puny_decode <- function(s)
{
  .Call("unpuny", s, PACKAGE="urltools")
}
