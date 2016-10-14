context("Check punycode handling")

testthat::test_that("Simple punycode domain encoding works", {
  testthat::expect_identical(puny_encode("https://www.bücher.com/foo"),
                             "https://www.xn--bcher-kva.com/foo")
})

testthat::test_that("Punycode domain encoding works with fragmentary paths", {
  testthat::expect_identical(puny_encode("https://www.bücher.com/"),
                             "https://www.xn--bcher-kva.com/")
})

testthat::test_that("Punycode domain encoding works with ports", {
  testthat::expect_identical(puny_encode("https://www.bücher.com:80"),
                             "https://www.xn--bcher-kva.com:80")
})

testthat::test_that("Punycode domain encoding returns an NA on NAs", {
  testthat::expect_true(is.na(puny_encode(NA_character_)))
})

testthat::test_that("Simple punycode domain decoding works", {
  testthat::expect_identical(puny_decode("https://www.xn--bcher-kva.com/foo"),
                             "https://www.bücher.com/foo")
})

testthat::test_that("Punycode domain decoding works with fragmentary paths", {
  testthat::expect_identical(puny_decode("https://www.xn--bcher-kva.com/"),
                             "https://www.bücher.com/")
})

testthat::test_that("Punycode domain decoding works with ports", {
  testthat::expect_identical(puny_decode("https://www.xn--bcher-kva.com:80"),
                             "https://www.bücher.com:80")
})

testthat::test_that("Punycode domain decoding returns an NA on NAs", {
  testthat::expect_true(is.na(puny_decode(NA_character_)))
})

testthat::test_that("Punycode domain decoding returns an NA on invalid entries", {
  testthat::expect_true(is.na(suppressWarnings(puny_decode("xn--9"))))
})

testthat::test_that("Punycode domain decoding warns on invalid entries", {
  testthat::expect_warning(puny_decode("xn--9"))
})