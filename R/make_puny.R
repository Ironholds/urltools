#' make_puny
#' 
#' Encode a UTF-8 string to its punycode representation.  See references
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
#' @export
make_puny <- function(s)
{
  .Call("make_puny", s, PACKAGE="urltools")
}

