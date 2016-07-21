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
#' @seealso \code{\link{suffix_extract}} for extracting suffixes from domain names.
#'
#' @usage data(suffix_dataset)
#' @note Last updated 2016-07-20.
#' @format A vector of 7430 elements.
"suffix_dataset"

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
#' #Using internal parsing
#' suffix_extract("http://en.wikipedia.org")
#'
#' @importFrom triebeard trie longest_match
#' @export
suffix_extract <- function(domains){
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
}