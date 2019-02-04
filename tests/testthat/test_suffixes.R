context("Test suffix extraction")

test_that("Suffix extraction works with simple domains",{
  result <- suffix_extract("en.wikipedia.org")
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
  expect_that(nrow(result), equals(1))
  
  expect_that(result$subdomain[1], equals("en"))
  expect_that(result$domain[1], equals("wikipedia"))
  expect_that(result$suffix[1], equals("org"))
})

test_that("Suffix extraction works with multiple domains",{
  result <- suffix_extract(c("en.wikipedia.org","en.wikipedia.org"))
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
  expect_that(nrow(result), equals(2))
  
  expect_that(result$subdomain[1], equals("en"))
  expect_that(result$domain[1], equals("wikipedia"))
  expect_that(result$suffix[1], equals("org"))
  expect_that(result$subdomain[2], equals("en"))
  expect_that(result$domain[2], equals("wikipedia"))
  expect_that(result$suffix[2], equals("org"))
})

test_that("Suffix extraction works when the domain is the same as the suffix",{
  result <- suffix_extract(c("googleapis.com", "myapi.googleapis.com"))
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
  expect_that(nrow(result), equals(2))
  
  expect_equal(result$subdomain[1], NA_character_)
  expect_equal(result$domain[1], NA_character_)
  expect_equal(result$suffix[1], "googleapis.com")
  expect_equal(result$subdomain[2], NA_character_)
  expect_equal(result$domain[2], "myapi")
  expect_equal(result$suffix[2], "googleapis.com")
})

test_that("Suffix extraction works where domains/suffixes overlap", {
  result <- suffix_extract(domain("http://www.converse.com")) # could be se.com or .com
  expect_equal(result$subdomain[1], "www")
  expect_equal(result$domain[1], "converse")
  expect_equal(result$suffix[1], "com")
})

test_that("Suffix extraction works when the domain matches a wildcard suffix",{
  result <- suffix_extract(c("banana.bd", "banana.boat.bd"))
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
  expect_that(nrow(result), equals(2))
  
  expect_equal(result$subdomain[1], NA_character_)
  expect_equal(result$domain[1], NA_character_)
  expect_equal(result$suffix[1], "banana.bd")
  expect_equal(result$subdomain[2], NA_character_)
  expect_equal(result$domain[2], "banana")
  expect_equal(result$suffix[2], "boat.bd")
})

test_that("Suffix extraction works when the domain matches a wildcard suffix and has subdomains",{
  result <- suffix_extract(c("foo.bar.banana.bd"))
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
  expect_that(nrow(result), equals(1))
  expect_equal(result$subdomain[1], "foo")
  expect_equal(result$domain[1], "bar")
  expect_equal(result$suffix[1], "banana.bd")
})


# test_that("Suffix extraction works with new suffixes",{
#   result <- suffix_extract("en.wikipedia.org", suffix_refresh())
#   expect_that(ncol(result), equals(4))
#   expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
#   expect_that(nrow(result), equals(1))
#   
#   expect_that(result$subdomain[1], equals("en"))
#   expect_that(result$domain[1], equals("wikipedia"))
#   expect_that(result$suffix[1], equals("org"))
# })
# 
# test_that("Suffix extraction works with an arbitrary suffixes database (to ensure it is loading it)",{
#   result <- suffix_extract(c("is-this-a.bananaboat", "en.wikipedia.org"), data.frame(suffixes = "bananaboat"))
#   expect_that(ncol(result), equals(4))
#   expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
#   expect_that(nrow(result), equals(2))
#   
#   expect_equal(result$subdomain[1], NA_character_)
#   expect_equal(result$domain[1], "is-this-a")
#   expect_equal(result$suffix[1], "bananaboat")
#   expect_equal(result$subdomain[2], NA_character_)
#   expect_equal(result$domain[2], NA_character_)
#   expect_equal(result$suffix[2], NA_character_)
# })

test_that("Suffix extraction is back to normal using the internal database when it receives suffixes=NULL",{
  result <- suffix_extract("en.wikipedia.org")
  expect_that(ncol(result), equals(4))
  expect_that(names(result), equals(c("host","subdomain","domain","suffix")))
  expect_that(nrow(result), equals(1))
  
  expect_that(result$subdomain[1], equals("en"))
  expect_that(result$domain[1], equals("wikipedia"))
  expect_that(result$suffix[1], equals("org"))
})