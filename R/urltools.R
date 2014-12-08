#' @title Tools for handling URLs
#' @name urltools
#' @description This package provides functions for URL encoding and decoding, parsing, and parameter extraction.
#'              The implementations are both fast and entirely vectorised It is intended to be useful for people
#'              making requests who need a fast, reliable encoding methods, and analysts dealing with data such
#'              as server-side logs.
#' @useDynLib urltools
#' @importFrom Rcpp sourceCpp
#' @docType package
#' @aliases urltools urltools-package
NULL