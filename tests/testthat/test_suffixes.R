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

test_that("Suffix extraction works with multiple domains",{
  result <- suffix_extract(c("en.wikipedia.org","en.wikipedia.org"))
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","tld")))
  expect_that(nrow(result), equals(2))
  
  expect_that(result$subdomain[1], equals("en"))
  expect_that(result$domain[1], equals("wikipedia"))
  expect_that(result$tld[1], equals("org"))
  expect_that(result$subdomain[2], equals("en"))
  expect_that(result$domain[2], equals("wikipedia"))
  expect_that(result$tld[2], equals("org"))
})