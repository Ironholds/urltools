context("URL encoding tests")

test_that("Check encoding doesn't encode the scheme", {
  expect_that(url_encode("https://"), equals("https://"))
})

test_that("Check encoding does encode subsequent slashes", {
  expect_that(url_encode("https:////"), equals("https://%2f%2f"))
})

test_that("Check decoding handles ASCII NULs", {
  expect_that(url_decode("0;%20@%gIL"), equals("0; @"))
})

test_that("Check decoding and encoding are equivalent", {
  
  url <- "Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG%2f120px-Hinrichtung_auf_dem_Altst%c3%a4dter_Ring.JPG"
  decoded_url <- "Hinrichtung_auf_dem_Altstädter_Ring.JPG/120px-Hinrichtung_auf_dem_Altstädter_Ring.JPG"
  expect_that(url_decode(url), equals(decoded_url))
  expect_that(url_encode(decoded_url), equals(url))
  
})