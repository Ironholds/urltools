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

test_that("URLs with parameters but no paths work", {
  url <- url_parse("http://www.nextpedition.com?inav=menu_travel_nextpedition")
  expect_true(url$domain[1] == "www.nextpedition.com")
  expect_true(url$port[1] == "")
  expect_true(url$path[1] == "")
  expect_true(url$parameter[1] == "inav=menu_travel_nextpedition")
})

test_that("Setting parameter values works", {
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php", "baz", "quorn") ==
              "https://en.wikipedia.org/wiki/api.php?baz=quorn")
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php?foo=bar&baz=qux", "baz", "quorn") ==
                "https://en.wikipedia.org/wiki/api.php?foo=bar&baz=quorn")
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php?foo=bar", "baz", "quorn") ==
                "https://en.wikipedia.org/wiki/api.php?foo=bar&baz=quorn")
})