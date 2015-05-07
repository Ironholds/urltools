#'@title refresh the stored public suffix dataset
#'
#'@description \code{urltools} comes with an inbuilt
#'dataset of public suffixes, \code{\link{suffix_dataset}}.
#'This is used in \code{\link{suffix_extract}} to identify the top-level domain
#'within a particular domain name.
#'
#'While updates to the dataset will be included in each new package release,
#'there's going to be a gap between changes to TLDs and changes to the package.
#'Accordingly, the package also includes \code{\link{suffix_refresh}}, which refreshes
#'this dataset on the user end. This requires CURL (the library, not the
#'R package) on your system to function.
#'
#'@return TRUE if this worked; an error otherwise.
#'
#'@seealso \code{\link{suffix_extract}} to extract suffixes from domain names,
#'or \code{\link{suffix_dataset}} for the dataset itself.
#'
#'@examples
#'\dontrun{
#'suffix_refresh()
#'}
suffix_refresh <- function(){
  
  #Read in and filter
  connection <- url("https://www.publicsuffix.org/list/effective_tld_names.dat", method = "libcurl")
  results <- readLines(connection)
  close(connection)
  results <- results[!grepl(x = results, pattern = "//", fixed = TRUE) & !results == ""]
  results <- paste0(".", results)
  
  #Sort by the number of periods
  match_order <- order(unlist(lapply(strsplit(x = results, split = ".", fixed = TRUE), length)),
                       decreasing = TRUE)
  suffix_dataset <- results[match_order]
  save(suffix_dataset, file = system.file("data/suffix_dataset.rda", package = "urltools"))
  return(TRUE)
}

#' @title Dataset of public suffixes
#' @description This dataset contains a registry of public suffixes, as retrieved from
#' and defined by the \href{https://publicsuffix.org/}{public suffix list}. It is
#' sorted by how many periods(".") appear in the suffix, to optimise it for
#' \code{\link{suffix_extract}}.
#'
#' @docType data
#' @keywords datasets
#' @name suffix_dataset
#'
#' @seealso \code{\link{suffix_refresh}} for updating the dataset, and
#' \code{\link{suffix_extract}} for extracting suffixes from domain names.
#'
#' @usage data(suffix_dataset)
#' @note Last updated 2015-05-06.
#' @format A vector of 7430 elements.
NULL

suffix_extract <- function(domains){
  load(system.file("data/suffix_dataset.rda", package = "urltools"))
  suffix_dataset <- suffix_dataset[order(1:length(suffix_dataset), decreasing=T)]
  return(suffix_extract_(domains, suffix_dataset))
}