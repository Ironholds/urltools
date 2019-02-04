#' @title Dataset of public suffixes
#' @description This dataset contains a registry of public suffixes, as retrieved from
#' and defined by the \href{https://publicsuffix.org/}{public suffix list}. It is
#' sorted by how many periods(".") appear in the suffix, to optimise it for
#' \code{\link{suffix_extract}}.  It is a data.frame with two columns, the first is
#' the list of suffixes and the second is our best guess at the comment or owner 
#' associated with the particular suffix. 
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
#' @note Last updated 2016-07-31.
#' @format A data.frame of 8030 rows and 2 columns
"suffix_dataset"

#'@title Retrieve a public suffix dataset
#'
#'@description \code{urltools} comes with an inbuilt
#'dataset of public suffixes, \code{\link{suffix_dataset}}.
#'This is used in \code{\link{suffix_extract}} to identify the top-level domain
#'within a particular domain name.
#'
#'While updates to the dataset will be included in each new package release,
#'there's going to be a gap between changes to the suffixes list and changes to the package.
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
  results <- readLines(connection, encoding = "UTF-8")
  close(connection)
  
  # making an assumption that sections are broken by blank lines
  blank <- which(results == "")
  # and gotta know where the comments are
  comments <- grep(pattern = "^//", x=results)
  
  # if the file doesn't end on a blank line, stick an ending on there.
  if (blank[length(blank)] < length(results)) {
    blank <- c(blank, length(results)+1)
  }
  # now break up each section into a list
  # grab right after the blank line and right before the next blank line.
  suffix_dataset <- do.call(rbind, lapply(seq(length(blank) - 1), function(i) {
    # these are the lines in the current block
    lines <- seq(blank[i] + 1, blank[i + 1] - 1)
    # assume there is nothing in the block
    rez <- NULL
    # the lines of text in this block
    suff <- results[lines]
    # of which these are the comments
    iscomment <- lines %in% comments
    # and check if we have any results 
    # append the first comment at the top of the block only.
    if(length(suff[!iscomment])) {
      rez <- data.frame(suffixes = suff[!iscomment],
                 comments = suff[which(iscomment)[1]], stringsAsFactors = FALSE)
    }
    return(rez)
  }))
  ## this is the old way
  #suffix_dataset <- results[!grepl(x = results, pattern = "//", fixed = TRUE) & !results == ""]

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
#' \dontrun{
#' #Relying on a fresh version of the suffix dataset
#' suffix_extract(domain("http://en.wikipedia.org"), suffix_refresh())
#' }
#' 
#' @importFrom triebeard trie longest_match
#' @export
suffix_extract <- function(domains, suffixes = NULL){
  if(!is.null(suffixes)){
    # check if suffixes is a data.frame, and stop if column not found
    if(is.data.frame(suffixes)) {
      if ("suffixes" %in% colnames(suffixes)) {
        suffixes <- suffixes$suffixes
      } else {
        stop("Expected column named \"suffixes\" in suffixes data.frame")
      }
    }
    holding <- suffix_load(suffixes)
  } else {
    holding <- list(suff_trie = urltools_env$suff_trie,
                    is_wildcard = urltools_env$is_wildcard,
                    cleaned_suffixes = urltools_env$cleaned_suffixes)
  }
  
  rev_domains <- reverse_strings(tolower(domains))
  matched_suffixes <- triebeard::longest_match(holding$suff_trie, rev_domains)
  has_wildcard <- matched_suffixes %in% holding$is_wildcard
  is_suffix <- domains %in% holding$cleaned_suffixes
  return(finalise_suffixes(domains, matched_suffixes, has_wildcard, is_suffix))
}

#' @title Dataset of top-level domains (TLDs)
#' @description This dataset contains a registry of top-level domains, as retrieved from
#' and defined by the \href{http://data.iana.org/TLD/tlds-alpha-by-domain.txt}{IANA}.
#' 
#' @docType data
#' @keywords datasets
#' @name tld_dataset
#'
#' @seealso \code{\link{tld_extract}} for extracting TLDs from domain names,
#' and \code{\link{tld_refresh}} to get an updated version of this dataset.
#'
#' @usage data(tld_dataset)
#' @note Last updated 2016-07-20.
#' @format A vector of 1275 elements.
"tld_dataset"

#'@title Retrieve a TLD dataset
#'
#'@description \code{urltools} comes with an inbuilt
#'dataset of top level domains (TLDs), \code{\link{tld_dataset}}.
#'This is used in \code{\link{tld_extract}} to identify the top-level domain
#'within a particular domain name.
#'
#'While updates to the dataset will be included in each new package release,
#'there's going to be a gap between changes to TLDs and changes to the package.
#'Accordingly, the package also includes \code{tld_refresh}, which generates
#'and returns a \emph{fresh} version of the dataset. This can then be passed through
#'to \code{\link{tld_extract}}.
#'
#'@return a dataset equivalent in format to \code{\link{tld_dataset}}.
#'
#'@seealso \code{\link{tld_extract}} to extract suffixes from domain names,
#'or \code{\link{tld_dataset}} for the inbuilt, default version of the data.
#'
#'@examples
#'\dontrun{
#'new_tlds <- tld_refresh()
#'}
#'
#'@export
tld_refresh <- function(){
  raw_tlds <- readLines("http://data.iana.org/TLD/tlds-alpha-by-domain.txt", warn = FALSE)
  raw_tlds <- tolower(raw_tlds[!grepl(x = raw_tlds, pattern = "#", fixed = TRUE)])
  return(raw_tlds)
}

#'@title Extract TLDs
#'@description \code{tld_extract} extracts the top-level domain (TLD) from
#'a vector of domain names. This is distinct from the suffixes, extracted with
#'\code{\link{suffix_extract}}; TLDs are \emph{top} level, while suffixes are just
#'domains through which internet users can publicly register domains (the difference
#'between \code{.org.uk} and \code{.uk}).
#'
#'@param domains a vector of domains, retrieved through \code{\link{url_parse}} or
#'\code{\link{domain}}.
#'
#'@param tlds a dataset of TLDs. If NULL (the default), \code{tld_extract} relies
#'on urltools' \code{\link{tld_dataset}}; otherwise, you can pass in the result of
#'\code{\link{tld_refresh}}.
#'
#'@return a data.frame of two columns: \code{domain}, with the original domain names,
#'and \code{tld}, the identified TLD from the domain.
#'
#'@examples
#'# Using the inbuilt dataset
#'domains <- domain("https://en.wikipedia.org/wiki/Main_Page")
#'tld_extract(domains)
#'
#'# Using a refreshed one
#'tld_extract(domains, tld_refresh())
#'
#'@seealso \code{\link{suffix_extract}} for retrieving suffixes (distinct from TLDs).
#'
#'@export
tld_extract <- function(domains, tlds = NULL){
  if(is.null(tlds)){
    tlds <- urltools::tld_dataset
  }
  guessed_tlds <- tld_extract_(tolower(domains))
  guessed_tlds[!guessed_tlds %in% tlds] <- NA
  return(data.frame(domain = domains, tld = guessed_tlds, stringsAsFactors = FALSE))
}

#'@title Extract hosts
#'@description \code{host_extract} extracts the host from
#'a vector of domain names. A host isn't the same as a domain - it could be
#'the subdomain, if there are one or more subdomains. The host of \code{en.wikipedia.org}
#'is \code{en}, while the host of \code{wikipedia.org} is \code{wikipedia}.
#'
#'@param domains a vector of domains, retrieved through \code{\link{url_parse}} or
#'\code{\link{domain}}.
#'
#'@return a data.frame of two columns: \code{domain}, with the original domain names,
#'and \code{host}, the identified host from the domain.
#'
#'@examples
#'# With subdomains
#'has_subdomain <- domain("https://en.wikipedia.org/wiki/Main_Page")
#'host_extract(has_subdomain)
#'
#'# Without
#'no_subdomain <- domain("https://ironholds.org/projects/r_shiny/")
#'host_extract(no_subdomain)
#'@export
host_extract <- function(domains){
  return(data.frame(domain = domains, host = host_extract_(domains), stringsAsFactors = FALSE))
}