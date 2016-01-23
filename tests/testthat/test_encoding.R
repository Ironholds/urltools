context("URL encoding tests")

test_that("Check encoding doesn't encode the scheme", {
  expect_that(url_encode("https://"), equals("https://"))
})

test_that("Check encoding does does not encode pre-path slashes", {
  expect_that(url_encode("https://foo.org/bar/"), equals("https://foo.org/bar%2f"))
})

test_that("Check encoding can handle NAs", {
  expect_that(url_encode(c("https://foo.org/bar/", NA)), equals(c("https://foo.org/bar%2f", NA)))
})

test_that("Check decoding can handle NAs", {
  expect_that(url_decode(c("https://foo.org/bar%2f", NA)), equals(c("https://foo.org/bar/", NA)))
})

test_that("Check decoding and encoding are equivalent", {
  
  url <- "Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG%2f120px-Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG"
  decoded_url <- "Hinrichtung_auf_dem_Altstädter_Ring.JPG/120px-Hinrichtung_auf_dem_Altstädter_Ring.JPG"
  expect_that(url_decode(url), equals(decoded_url))
  expect_that(url_encode(decoded_url), equals(url))
  
})