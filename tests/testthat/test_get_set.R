context("Component get/set tests")

test_that("Check elements can be retrieved", {
  url <- "https://www.google.com:80/foo.php?api_params=turnip#ending"
  testthat::expect_equal(scheme(url), "https")
  testthat::expect_equal(domain(url), "www.google.com")
  testthat::expect_equal(port(url), "80")
  testthat::expect_equal(path(url), "foo.php")
  testthat::expect_equal(parameters(url), "api_params=turnip")
  testthat::expect_equal(fragment(url), "ending")
})

test_that("Check elements can be retrieved with NAs", {
  url <- as.character(NA)
  testthat::expect_equal(is.na(scheme(url)), TRUE)
  testthat::expect_equal(is.na(domain(url)), TRUE)
  testthat::expect_equal(is.na(port(url)), TRUE)
  testthat::expect_equal(is.na(path(url)), TRUE)
  testthat::expect_equal(is.na(parameters(url)), TRUE)
  testthat::expect_equal(is.na(fragment(url)), TRUE)
})

test_that("Check elements can be set", {
  url <- "https://www.google.com:80/foo.php?api_params=turnip#ending"
  scheme(url) <- "http"
  testthat::expect_equal(scheme(url), "http")
  domain(url) <- "www.wikipedia.org"
  testthat::expect_equal(domain(url), "www.wikipedia.org")
  port(url) <- "23"
  testthat::expect_equal(port(url), "23")
  path(url) <- "bar.php"
  testthat::expect_equal(path(url), "bar.php")
  parameters(url) <- "api_params=manic"
  testthat::expect_equal(parameters(url), "api_params=manic")
  fragment(url) <- "beginning"
  testthat::expect_equal(fragment(url), "beginning")
})
