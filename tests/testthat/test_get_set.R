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

test_that("Check elements can be set with NAs", {
  url <- "https://www.google.com:80/"
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

test_that("Assigning NA with get will NA a URL", {
  url <- "https://www.google.com:80/"
  port(url) <- NA_character_
  testthat::expect_true(is.na(url))
})

test_that("Removing components with a NULL works", {
  url <- "https://www.google.com:80/foo.php?api_params=turnip#ending"
  fragment(url) <- NULL
  testthat::expect_equal(url,
                         "https://www.google.com:80/foo.php?api_params=turnip")
  parameters(url) <- NULL
  testthat::expect_equal(url,
                         "https://www.google.com:80/foo.php")
  path(url) <- NULL
  testthat::expect_equal(url,
                         "https://www.google.com:80")
  port(url) <- NULL
  testthat::expect_equal(url, "https://www.google.com")
})

test_that("Removing non-removable components throws an error", {
  
  url <- "https://en.wikipedia.org/foo.php"
  testthat::expect_error({
    scheme(url) <- NULL
  })
  testthat::expect_error({
    domain(url) <- NULL
  })
})

test_that("Check multiple elements can be set", {
  url <- "https://www.google.com:80/foo.php?api_params=turnip#ending"
  url <- c(url, url)
  scheme(url) <- c("http", "ftp")
  testthat::expect_equal(scheme(url), c("http", "ftp"))
  domain(url) <- c("www.wikipedia.org", "google.com")
  testthat::expect_equal(domain(url), c("www.wikipedia.org",
                                        "google.com"))
  port(url) <- c("23", "86")
  testthat::expect_equal(port(url), c("23", "86"))
  path(url) <- c("bar.php", "baz.html")
  testthat::expect_equal(path(url), c("bar.php", "baz.html"))
  parameters(url) <- c("api_params=manic", "api_params=street")
  testthat::expect_equal(parameters(url), c("api_params=manic", "api_params=street"))
  fragment(url) <- c("beginning", "end")
  testthat::expect_equal(fragment(url), c("beginning", "end"))
})

test_that("Check elements can be set with extraneous separators", {
  url <- "https://www.wikipedia.org:80/bar.php?api_params=manic#beginning"
  backup <- url
  scheme(url) <- "https://"
  testthat::expect_equal(url, backup)
  port(url) <- ":80"
  testthat::expect_equal(url, backup)
  path(url) <- "/bar.php"
  testthat::expect_equal(url, backup)
  parameters(url) <- "?api_params=manic"
  testthat::expect_equal(url, backup)
  fragment(url) <- "#beginning"
  testthat::expect_equal(url, backup)
})