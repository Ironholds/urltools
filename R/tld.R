#'@title refresh the stored Top Level Domain dataset
#'
#'@description \code{urltools} comes with an inbuilt
#'dataset of Top Level Domains (TLDs), \code{\link{tld_dataset}}.
#'This is used in \code{\link{tld_extract}} to identify the top-level domain
#'within a particular domain name.
#'
#'While updates to the dataset will be included in each new package release,
#'there's going to be a gap between changes to TLDs and changes to the package.
#'Accordingly, the package also includes \code{\link{tld_refresh}}, which refreshes
#'this dataset on the user end. This requires CURL (the library, not the
#'R package) on your system to function.
#'
#'@return TRUE if this worked; an error otherwise.
#'
#'@seealso \code{\link{tld_extract}} to extract TLDs from domain names,
#'or \code{\link{tld_dataset}} for the dataset itself.
#'
#'@examples
#'\dontrun{
#'tld_refresh()
#'}
tld_refresh <- function(){
  
  #Read in and filter
  connection <- url("https://www.publicsuffix.org/list/effective_tld_names.dat", method = "libcurl")
  results <- readLines(connection)
  close(connection)
  results <- results[!grepl(x = results, pattern = "//", fixed = TRUE) & !results == ""]
  results <- paste0(".", results)
  
  #Sort by the number of periods
  match_order <- order(unlist(lapply(strsplit(x = results, split = ".", fixed = TRUE), length)),
                       decreasing = TRUE)
  tld_dataset <- results[match_order]
  save(tld_dataset, file = system.file("data/tld_dataset.rda", package = "urltools"))
  return(TRUE)
}