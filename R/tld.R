#'@title Update the locally-stored vector of top-level domains
#'
#'@description \code{urltools} includes a vector of accepted top-level domains,
#'\code{\link{domain_names}}, which is used in \code{\link{domain_parse}}.
#'These should be updated with every new package release, but can
#'also be updated by the user by calling this function.
#'
#'This functionality (and this update function) was extensively influenced by Jay Jacobs'
#'\href{https://github.com/jayjacobs/tldextract}{tldextract} package.
#'
#'@seealso \code{\link{domain_parse}}, which uses this vector, and \code{\link{domain_names}}
#'for the dataset itself.
#'
#'@examples
#'\dontrun{
#'update_domains()
#'}
#'
#'@export
update_domains <- function(){
  con <- url("http://data.iana.org/TLD/tlds-alpha-by-domain.txt")
  raw_results <- readLines(con)
  close(con)
  parsed_results <- tolower(raw_results[2:length(raw_results)])
  domain_names <- parsed_results[!grepl(x = parsed_results, pattern = "^XN--")]
  regex_ready_domain_names <- paste0("\\.",domain_names,"")
  save(domain_names, regex_ready_domain_names,
       file = file.path(find.package("urltools"),"data","domain_names.rda"))
  return(invisible())
}

#' @title top-level domain names
#'
#' @description a vector containing the accepted top-level domain names, retrieved from
#' \href{http://data.iana.org/TLD/tlds-alpha-by-domain.txt}{the IANA}.
#'
#' @format two character vectors; one, \code{domain_names}, contains the lower-cased
#' TLDs. The other, \code{regex_ready_domain_names}, stores them in the format
#' "\\.domain$" to make them easily applicable to hostname parsing.
#' 
#' @name domain_names
NULL