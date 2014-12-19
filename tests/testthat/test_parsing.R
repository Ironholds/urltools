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

test_that("parameter value extraction works", {
  
  original_url <- "https://en.wikipedia.org/w/api.php?action=sitematrix&format=xml&smstate=all&smsiteprop=url|dbname|code|sitename&smlimit=2000"
  expect_that(url_extract_param(original_url,"format"), equals("xml"))
  
})

test_that("parameter value replacement works", {
  
  original_url <- "https://en.wikipedia.org/w/api.php?action=sitematrix&format=xml&smstate=all&smsiteprop=url|dbname|code|sitename&smlimit=2000"
  expected_url <- "https://en.wikipedia.org/w/api.php?action=sitematrix&format=json&smstate=all&smsiteprop=url|dbname|code|sitename&smlimit=2000"
  expect_that(url_replace_param(original_url,"format","json"), equals(expected_url))
})
