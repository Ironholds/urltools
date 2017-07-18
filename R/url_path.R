#' Construct URL Path
#' 
#' Construct a URL from path components like \code{file.path}
#'
#' @param x a URL, or vector of URLs
#' @param ... character vectors
#' 
#' @examples
#' 
#' url_path("https://github.com", c('Ironholds', 'nfultz'), 'urltools')
#' 
#' @export
#'
url_path <- function(x, ...) {
  p <- path(x)
  p[is.na(p)] = '.'
  p <- file.path(p, ..., fsep='/')
  x <- mapply(`path<-`, x, p, USE.NAMES = FALSE)
  gsub('/[.]/', '/', x)
}