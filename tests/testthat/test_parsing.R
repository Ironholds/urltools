context("URL parsing tests")

test_that("Check parsing identifies each RfC element", {
  
  data <- url_parse("https://www.google.com:80/foo.php?api_params=turnip#ending")
  expect_that(ncol(data), equals(6))
  expect_that(names(data), equals(c("scheme","domain","port","path","parameter","fragment")))
  expect_that(data$scheme[1], equals("https"))
  expect_that(data$domain[1], equals("www.google.com"))
  expect_that(data$port[1], equals("80"))
  expect_that(data$path[1], equals("foo.php"))
  expect_that(data$parameter[1], equals("api_params=turnip"))
  expect_that(data$fragment[1], equals("ending"))
})

test_that("Check parsing can handle missing elements", {
  
  data <- url_parse("https://www.google.com/foo.php?api_params=turnip#ending")
  expect_that(ncol(data), equals(6))
  expect_that(names(data), equals(c("scheme","domain","port","path","parameter","fragment")))
  expect_that(data$scheme[1], equals("https"))
  expect_that(data$domain[1], equals("www.google.com"))
  expect_that(data$port[1], equals(""))
  expect_that(data$path[1], equals("foo.php"))
  expect_that(data$parameter[1], equals("api_params=turnip"))
  expect_that(data$fragment[1], equals("ending"))
})

test_that("Parsing does not up and die and misplace the fragment",{
  data <- url_parse("http://www.yeastgenome.org/locus/S000005366/overview#protein")
  expect_that(data$fragment[1], equals("protein"))
})

test_that("Composing works",{
  url <- "http://foo.bar.baz/qux/"
  amended_url <- url_compose(url_parse(url))
  expect_that(url, equals(amended_url))
})

test_that("Port handling works", {
  url <- "https://en.wikipedia.org:4000/wiki/api.php"
  expect_that(port(url), equals("4000"))
  expect_that(path(url), equals("wiki/api.php"))
  url <- "https://en.wikipedia.org:4000"
  expect_that(port(url), equals("4000"))
  expect_that(path(url), equals(""))
  url <- "https://en.wikipedia.org:4000/"
  expect_that(port(url), equals("4000"))
  expect_that(path(url), equals(""))
  url <- "https://en.wikipedia.org:4000?foo=bar"
  expect_that(port(url), equals("4000"))
  expect_that(path(url), equals(""))
  expect_that(parameters(url), equals("foo=bar"))
})