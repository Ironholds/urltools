context("Test parameter manipulation")

test_that("Parameter parsing can handle multiple, non-existent and pre-trailing parameters",{
  urls <- c("https://www.google.com:80/foo.php?api_params=parsable&this_parameter=selfreferencing&hiphop=awesome",
            "https://www.google.com:80/foo.php?api_params=parsable&this_parameter=selfreferencing&hiphop=awesome#foob",
            "https://www.google.com:80/foo.php?this_parameter=selfreferencing&hiphop=awesome")
  results <- param_get(urls, c("api_params","hiphop"))
  expect_that(results[1:2,1], equals(c("parsable","parsable")))
  expect_true(is.na(results[3,1]))
})

test_that("Parameter parsing works where the parameter appears earlier in the URL", {
  url <- param_get("www.housetrip.es/tos-de-vacaciones/geo?from=01/04/2015&guests=4&to=05/04/2015","to")
  expect_that(ncol(url), equals(1))
  expect_that(url$to[1], equals("05/04/2015"))
})

test_that("Default argument will get all parameter keys", {
  url <- param_get("www.housetrip.es/tos-de-vacaciones/geo?from=01/04/2015&guests=4&to=05/04/2015")
  df <- data.frame("from"="01/04/2015", "guests"="4", "to"="05/04/2015", stringsAsFactors = FALSE)
  testthat::expect_equivalent(url, df)
})

test_that("vectorized get all keys produces NA appropriately", {
  urls <- c("www.housetrip.es/tos-de-vacaciones/geo?guests=4&to=05/04/2015",
            "www.housetrip.es/tos-de-vacaciones/geo?from=01/04/2015&guests=8")

  pars <- param_get(urls)
  df <- data.frame(stringsAsFactors = FALSE,
                   from = c(NA, "01/04/2015"),
                   guests = c("4", "8"),
                   to = c("05/04/2015", NA))

  expect_equivalent(pars, df)
})

test_that("parameter get deals with escaped ampersands and fragments in field values", {
  par <- param_get("http://host/query?foo=bar&amp;baz&amp=nonsense#a=1&b=2", "amp")
  expect_equivalent(par, data.frame(amp="nonsense", stringsAsFactors=FALSE))
  
  par <- param_get("http://host/query?foo=bar&amp;baz&amp=nonsense#a=1&b=2")
  expect_equivalent(par, data.frame(amp="nonsense", foo="bar&amp;baz", stringsAsFactors=FALSE))
})

test_that("Setting parameter values works", {
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php", "baz", "quorn") ==
              "https://en.wikipedia.org/wiki/api.php?baz=quorn")
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php?foo=bar&baz=qux", "baz", "quorn") ==
                "https://en.wikipedia.org/wiki/api.php?foo=bar&baz=quorn")
  expect_true(param_set("https://en.wikipedia.org/wiki/api.php?foo=bar", "baz", "quorn") ==
                "https://en.wikipedia.org/wiki/api.php?foo=bar&baz=quorn")
})

test_that("Setting parameter values quietly fails with NA components", {
  url <- "https://en.wikipedia.org/api.php?action=query"
  expect_identical(url, param_set(url, "action", NA_character_))
  expect_true(is.na(param_set(NA_character_, "action", "foo")))
  expect_identical(url, param_set(url, NA_character_, "pageinfo"))
})

test_that("Setting parameter values works with partially-duplicative keys", {
  url <- "https://en.wikipedia.org/api.php"
  url <- param_set(url, "foo", "bar")
  url <- param_set(url, "oo", "baz")
  testthat::expect_equal(url,
                         "https://en.wikipedia.org/api.php?foo=bar&oo=baz")
})
test_that("Removing parameter entries quietly fails with NA components", {
  url <- "https://en.wikipedia.org/api.php?action=query"
  expect_identical(url, param_remove(url, "foo"))
  expect_true(is.na(param_remove(NA_character_, "action")))
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
