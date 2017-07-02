#' @title Tools for handling URLs
#' @name urltools
#' @description This package provides functions for URL encoding and decoding,
#' parsing, and parameter extraction, designed to be both fast and
#' entirely vectorised. It is intended to be useful for people dealing with
#' web-related datasets, such as server-side logs.
#' 
#' @seealso the \href{https://CRAN.R-project.org/package=urltools/vignettes/urltools.html}{package vignette}.
#' @useDynLib urltools, .registration = TRUE
#' @importFrom Rcpp sourceCpp
#' @docType package
#' @aliases urltools urltools-package
NULL