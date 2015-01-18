context("URL parsing tests")

test_that("Check parsing identifies each RfC element", {
  
  data <- unlist(url_parse("https://www.google.com:80/foo.php?api_params=turnip#ending"))
  expect_that(data[1], equals("https"))
  expect_that(data[2], equals("www.google.com"))
  expect_that(data[3], equals("80"))
  expect_that(data[4], equals("foo.php"))
  expect_that(data[5], equals("api_params=turnip"))
  expect_that(data[6], equals("ending"))
})

test_that("Check parsing can handle missing elements", {
  
  data <- unlist(url_parse("https://www.google.com/foo.php?api_params=turnip#ending"))
  expect_that(data[1], equals("https"))
  expect_that(data[2], equals("www.google.com"))
  expect_that(data[3], equals(""))
  expect_that(data[4], equals("foo.php"))
  expect_that(data[5], equals("api_params=turnip"))
  expect_that(data[6], equals("ending"))
})