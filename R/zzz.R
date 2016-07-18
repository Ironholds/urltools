urltools_env <- new.env(parent = emptyenv())

.onLoad <- function(...) {
  cleaned_suffixes <- gsub(x = urltools::suffix_dataset, pattern = "*.", replacement = "", fixed = TRUE)
  is_wildcard <- cleaned_suffixes[which(grepl(x = urltools::suffix_dataset, pattern = "*.", fixed = TRUE))]
  assign("is_wildcard", is_wildcard, envir = urltools_env)
  suff_trie <- triebeard::trie(keys = reverse_strings(cleaned_suffixes),
                               values = cleaned_suffixes)
  assign("suff_trie", suff_trie, envir = urltools_env)
}