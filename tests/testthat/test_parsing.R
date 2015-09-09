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

test_that("Parameter parsing can handle multiple, non-existent and pre-trailing parameters",{
  urls <- c("https://www.google.com:80/foo.php?api_params=parsable&this_parameter=selfreferencing&hiphop=awesome",
            "https://www.google.com:80/foo.php?api_params=parsable&this_parameter=selfreferencing&hiphop=awesome#foob",
            "https://www.google.com:80/foo.php?this_parameter=selfreferencing&hiphop=awesome")
  results <- url_parameters(urls, c("api_params","hiphop"))
  expect_that(results[,1], equals(c("parsable","parsable","")))
  
})

test_that("Parameter parsing works where the parameter appears earlier in the URL", {
  url <- url_parameters("www.housetrip.es/tos-de-vacaciones/geo?from=01/04/2015&guests=4&to=05/04/2015","to")
  expect_that(ncol(url), equals(1))
  expect_that(url$to[1], equals("05/04/2015"))
})

test_that("Composing works",{
  url <- "http://foo.bar.baz/qux/"
  amended_url <- url_compose(url_parse(url))
  expect_that(url, equals(amended_url))
})

test_that("URLs with parameters but no paths work", {
  url <- url_parse("http://www.nextpedition.com?inav=menu_travel_nextpedition")
  expect_true(url$domain[1] == "www.nextpedition.com")
  expect_true(url$port[1] == "")
  expect_true(url$path[1] == "")
  expect_true(url$parameter[1] == "inav=menu_travel_nextpedition")
})