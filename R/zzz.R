urltools_env <- new.env(parent = emptyenv())

suffix_load <- function(suffixes = NULL){
  if(is.null(suffixes)){
    suffixes <- urltools::suffix_dataset
  }
  cleaned_suffixes <- gsub(x = suffixes$suffixes, pattern = "*.", replacement = "", fixed = TRUE)
  is_wildcard <- cleaned_suffixes[which(grepl(x = suffixes$suffixes, pattern = "*.", fixed = TRUE))]
  suff_trie <- triebeard::trie(keys = reverse_strings(paste0(".", cleaned_suffixes)),
                               values = cleaned_suffixes)
  return(list(suff_trie = suff_trie,
              is_wildcard = is_wildcard,
              cleaned_suffixes = cleaned_suffixes))
  return(invisible())
}

.onLoad <- function(...) {
  holding <- suffix_load()
  assign("is_wildcard", holding$is_wildcard, envir = urltools_env)
  assign("cleaned_suffixes", holding$cleaned_suffixes, envir = urltools_env)
  assign("suff_trie", holding$suff_trie, envir = urltools_env)
}