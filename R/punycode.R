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
#' @references Punycode RFC \url{http://www.faqs.org/rfcs/rfc3492.html}
#' 
#' @name punycode
#' @rdname punycode
NULL

#' @rdname punycode
#' @export
topuny <- function(s)
{
  .Call("topuny", s, PACKAGE="urltools")
}

#' @rdname punycode
#' @export
unpuny <- function(s)
{
  .Call("unpuny", s, PACKAGE="urltools")
}
