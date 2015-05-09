context("Test suffix extraction")

test_that("Suffix extraction works with simple domains",{
  result <- suffix_extract("en.wikipedia.org")
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","tld")))
  expect_that(nrow(result), equals(1))
  
  expect_that(result$subdomain[1], equals("en"))
  expect_that(result$domain[1], equals("wikipedia"))
  expect_that(result$tld[1], equals("org"))
})