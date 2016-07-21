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
#' @seealso \code{\link{suffix_extract}} for extracting suffixes from domain names,
#' and \code{\link{suffix_refresh}} for getting a new, totally-up-to-date dataset
#' version.
#'
#' @usage data(suffix_dataset)
#' @note Last updated 2016-07-20.
#' @format A vector of 8020 elements.
"suffix_dataset"

#'@title Retrieve a public suffix dataset
#'
#'@description \code{urltools} comes with an inbuilt
#'dataset of public suffixes, \code{\link{suffix_dataset}}.
#'This is used in \code{\link{suffix_extract}} to identify the top-level domain
#'within a particular domain name.
#'
#'While updates to the dataset will be included in each new package release,
#'there's going to be a gap between changes to TLDs and changes to the package.
#'Accordingly, the package also includes \code{suffix_refresh}, which generates
#'and returns a \emph{fresh} version of the dataset. This can then be passed through
#'to \code{\link{suffix_extract}}.
#'
#'@return a dataset equivalent in format to \code{\link{suffix_dataset}}.
#'
#'@seealso \code{\link{suffix_extract}} to extract suffixes from domain names,
#'or \code{\link{suffix_dataset}} for the inbuilt, default version of the data.
#'
#'@examples
#'\dontrun{
#'new_suffixes <- suffix_refresh()
#'}
#'
#'@export
suffix_refresh <- function(){
  
  has_libcurl <- capabilities("libcurl")
  if(length(has_libcurl) == 0 || has_libcurl == FALSE){
    stop("libcurl support is needed for this function")
  }
  
  #Read in and filter
  connection <- url("https://www.publicsuffix.org/list/effective_tld_names.dat", method = "libcurl")
  results <- readLines(connection)
  close(connection)
  suffix_dataset <- results[!grepl(x = results, pattern = "//", fixed = TRUE) & !results == ""]

  #Return the user-friendly version
  return(suffix_dataset)
}

#' @title extract the suffix from domain names
#' @description domain names have suffixes - common endings that people
#' can or could register domains under. This includes things like ".org", but
#' also things like ".edu.co". A simple Top Level Domain list, as a
#' result, probably won't cut it.
#'
#' \code{\link{suffix_extract}} takes the list of public suffixes,
#' as maintained by Mozilla (see \code{\link{suffix_dataset}}) and
#' a vector of domain names, and produces a data.frame containing the
#' suffix that each domain uses, and the remaining fragment.
#'
#' @param domains a vector of damains, from \code{\link{domain}}
#' or \code{\link{url_parse}}. Alternately, full URLs can be provided
#' and will then be run through \code{\link{domain}} internally.
#'
#' @param suffixes a dataset of suffixes. By default, this is NULL and the function
#' relies on \code{\link{suffix_dataset}}. Optionally, if you want more updated
#' suffix data, you can provide the result of \code{\link{suffix_refresh}} for
#' this parameter.
#' 
#' @return a data.frame of four columns, "host" "subdomain", "domain" & "suffix".
#' "host" is what was passed in. "subdomain" is the subdomain of the suffix.
#' "domain" contains the part of the domain name that came before the matched suffix.
#' "suffix" is, well, the suffix.
#'
#' @seealso \code{\link{suffix_dataset}} for the dataset of suffixes.
#'
#' @examples
#'
#' # Using url_parse
#' domain_name <- url_parse("http://en.wikipedia.org")$domain
#' suffix_extract(domain_name)
#'
#' # Using domain()
#' domain_name <- domain("http://en.wikipedia.org")
#' suffix_extract(domain_name)
#'
#' #Relying on a fresh version of the suffix dataset
#' suffix_extract(domain("http://en.wikipedia.org"), suffix_refresh())
#' 
#' @importFrom triebeard trie longest_match
#' @export
suffix_extract <- function(domains, suffixes = NULL){
  if(!is.null(suffixes)){
    suffix_load(suffixes)
  }
  rev_domains <- reverse_strings(domains)
  matched_suffixes <- triebeard::longest_match(urltools_env$suff_trie, rev_domains)
  has_wildcard <- matched_suffixes %in% urltools_env$is_wildcard
  return(finalise_suffixes(domains, matched_suffixes, has_wildcard))
}

#' @title Dataset of top-level domains (TLDs)
#' @description This dataset contains a registry of top-level domains, as retrieved from
#' and defined by the \href{http://data.iana.org/TLD/tlds-alpha-by-domain.txt}{IANA}.
#' 
#' @docType data
#' @keywords datasets
#' @name tld_dataset
#'
#' @seealso \code{\link{suffix_extract}} for extracting suffixes from domain names.
#'
#' @usage data(tld_dataset)
#' @note Last updated 2016-07-20.
#' @format A vector of 1275 elements.
"tld_dataset"

get_tlds <- function(){
  raw_tlds <- readLines("http://data.iana.org/TLD/tlds-alpha-by-domain.txt", warn = FALSE)
  raw_tlds <- tolower(raw_tlds[!grepl(x = raw_tlds, pattern = "(#|--)")])
  return(raw_tlds)
}

tld_extract <- function(){
  
}