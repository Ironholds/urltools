context("Test parameter manipulation")

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

test_that("Setting parameter values works", {
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php", "baz", "quorn") ==
              "https://en.wikipedia.org/wiki/api.php?baz=quorn")
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php?foo=bar&baz=qux", "baz", "quorn") ==
                "https://en.wikipedia.org/wiki/api.php?foo=bar&baz=quorn")
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php?foo=bar", "baz", "quorn") ==
                "https://en.wikipedia.org/wiki/api.php?foo=bar&baz=quorn")
})

test_that("Removing parameter keys works", {
  expect_true(param_remove("https://en.wikipedia.org/api.php?baz=qux", "baz") ==
                "https://en.wikipedia.org/api.php")
})

test_that("Removing parameter keys works when there are multiple parameters in the URL", {
  expect_true(param_remove("https://en.wikipedia.org/api.php?baz=qux&foo=bar", "baz") ==
                "https://en.wikipedia.org/api.php?foo=bar")
})

test_that("Removing parameter keys works when there are multiple parameters to remove", {
  expect_true(param_remove("https://en.wikipedia.org/api.php?baz=qux&foo=bar", c("baz","foo")) ==
                "https://en.wikipedia.org/api.php")
})

test_that("Removing parameter keys works when there is no query", {
  expect_true(param_remove("https://en.wikipedia.org/api.php", "baz") ==
              "https://en.wikipedia.org/api.php")
})

