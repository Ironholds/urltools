#' @title Tools for handling URLs
#' @name urltools
#' @description This package provides functions for URL encoding and decoding,
#' parsing, and parameter extraction, designed to be both fast and
#' entirely vectorised. It is intended to be useful for people dealing with
#' web-related datasets, such as server-side logs.
#' 
#' @seealso the \href{http://cran.rstudio.com/web/packages/urltools/vignettes/urltools.html}{package vignette}.
#' @useDynLib urltools
#' @importFrom Rcpp sourceCpp
#' @docType package
#' @aliases urltools urltools-package
NULL

#'@title get the values of a URL's parameters (deprecated)
#'@description the old name for \code{\link{param_get}}; deprecated
#'in favour of that function.
#'
#'@export
url_parameters <- function(urls, parameter_names){
  return(param_get(urls, parameter_names))
}