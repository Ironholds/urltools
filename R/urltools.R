#' @title Tools for handling URLs
#' @name urltools
#' @description This package provides functions for URL encoding and decoding,
#' parsing, and parameter extraction, designed to be both fast and
#' entirely vectorised. It is intended to be useful for people dealing with
#' web-related datasets, such as server-side logs.
#' 
#' @seealso \link{url_decode}, \link{url_encode}, \link{extract_parameter}, \link{replace_parameter},
#' \link{parse_url}, \link{extract_host}
#' @useDynLib urltools
#' @importFrom Rcpp sourceCpp
#' @docType package
#' @aliases urltools urltools-package
NULL