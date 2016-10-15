context("Issues from censornet/HIBPwned")

test_that("Values are correctly disposed from memory",{
  memfn<-function(d=NULL){
    URL<-"https://test.com"
    if(!is.null(d)) URL<- urltools::param_set(URL,"q",urltools::url_encode(d))
    return(URL)
  }
  
  baseurl<-"https://test.com"
  expect_equal(memfn(),baseurl)
  expect_equal(memfn("blah"),paste0(baseurl,"?q=blah"))
  expect_equal(memfn(),baseurl)
})