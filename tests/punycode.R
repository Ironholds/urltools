library(urltools)


# Original inputs
input <- c(
  "bücher",
  "세계의모든사람들이한국어를이해한다면얼마나좋을까",
  "למההםפשוטלאמדבריםעברית",
  "他們爲什麽不說中文"
)
input <- enc2utf8(input)


# test encoding
test <- urltools:::topuny(input)
truth <- c(
  "bcher-kva",
  "989aomsvi5e83db1d2a355cv1e0vak1dwrv93d5xbh15a0dt30a5jpsd879ccm6fea98c",
  "4dbcagdahymbxekheh6e0a7fei0b",
  "ihqwctvzc91f659drss3x8bo0yb"
)

stopifnot(identical(test, truth))


# test decoding
test <- urltools:::unpuny(test)
test <- enc2utf8(test)

#stopifnot(identical(input, test))

