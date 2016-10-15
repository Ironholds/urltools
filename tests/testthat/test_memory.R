context("Avoid regressions around proxy objects")

test_that("Values are correctly disposed from memory",{
  memfn <- function(d = NULL){
    url <- "https://test.com"
    if(!is.null(d)){
      url <- urltools::param_set(url, "q" , urltools::url_encode(d))
     }
    return(url)
  }
  
  baseurl < -"https://test.com"
  expect_equal(memfn(), baseurl)
  expect_equal(memfn("blah"), paste0(baseurl, "?q=blah"))
  expect_equal(memfn(), baseurl)
})

test_that("Parameters correctly add to output",{
  outfn <- function(d = FALSE){
    url <- "https://test.com"
    if(d){
      url <- urltools::param_set(URL, "q", urltools::url_encode(d))
    }
    return(url)
  }
  
  baseurl <- "https://test.com"
  expect_equal(outfn(), baseurl)
  expect_equal(outfn(TRUE), paste0(baseurl, "?q=TRUE"))
})
