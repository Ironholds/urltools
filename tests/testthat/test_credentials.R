testthat::context("Test credential extraction and getting")

testthat::test_that("Credentials can be stripped", {
  testthat::expect_identical(strip_credentials("http://foo:bar@97.77.104.22:3128"), "http://97.77.104.22:3128")
})

testthat::test_that("Strings with invalidly-placed credentials are left alone", {
  testthat::expect_identical(strip_credentials("htt@p://foo:bar97.77.104.22:3128"), "htt@p://foo:bar97.77.104.22:3128")
})

testthat::test_that("Invalid URLs are left alone", {
  testthat::expect_identical(strip_credentials("foo:bar@97.77.104.22:3128"), "foo:bar@97.77.104.22:3128")
})

testthat::test_that("Non-objects are left alone", {
  testthat::expect_true(is.na(strip_credentials(NA_character_)))
})

testthat::test_that("Credentials can be retrieved", {
  data <- get_credentials("http://foo:bar@97.43.5421")
  testthat::expect_identical(data$username, "foo")
  testthat::expect_identical(data$authentication, "bar")
})

testthat::test_that("Strings with invalidly-placed credentials are left alone", {
  data <- get_credentials("htt@p://foo:bar97.77.104.22:3128")
  testthat::expect_true(is.na(data$username))
  testthat::expect_true(is.na(data$authentication))
})

testthat::test_that("Invalid URLs are left alone", {
  data <- get_credentials("foo:bar@97.77.104.22:3128")
  testthat::expect_true(is.na(data$username))
  testthat::expect_true(is.na(data$authentication))
})

testthat::test_that("Non-objects are left alone", {
  data <- get_credentials(NA_character_)
  testthat::expect_true(is.na(data$username))
  testthat::expect_true(is.na(data$authentication))
})