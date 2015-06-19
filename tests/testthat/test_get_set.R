context("Component get/set tests")

test_that("Check elements can be retrieved", {
  url <- "https://www.google.com:80/foo.php?api_params=turnip#ending"
  expect_that(scheme(url), equals("https"))
  expect_that(domain(url), equals("www.google.com"))
  expect_that(port(url), equals("80"))
  expect_that(path(url), equals("foo.php"))
  expect_that(parameters(url), equals("api_params=turnip"))
  expect_that(fragment(url), equals("ending"))
})

test_that("Check elements can be set", {
  url <- "https://www.google.com:80/foo.php?api_params=turnip#ending"
  scheme(url) <- "http"
  expect_that(scheme(url), equals("http"))
  domain(url) <- "www.wikipedia.org"
  expect_that(domain(url), equals("www.wikipedia.org"))
  port(url) <- "23"
  expect_that(port(url), equals("23"))
  path(url) <- "bar.php"
  expect_that(path(url), equals("bar.php"))
  parameters(url) <- "api_params=manic"
  expect_that(parameters(url), equals("api_params=manic"))
  fragment(url) <- "beginning"
  expect_that(fragment(url), equals("beginning"))
})
