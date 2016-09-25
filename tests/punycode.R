library(urltools)


# Original inputs
input <- c(
  "b\u00FCcher",
  "\uC138\uACC4\uC758\uBAA8\uB4E0\uC0AC\uB78C\uB4E4\uC774\uD55C\uAD6D\uC5B4\uB97C\uC774\uD574\uD55C\uB2E4\uBA74\uC5BC\uB9C8\uB098\uC88B\uC744\uAE4C",
  "\u05DC\u05DE\u05D4\u05D4\u05DD\u05E4\u05E9\u05D5\u05D8\u05DC\u05D0\u05DE\u05D3\u05D1\u05E8\u05D9\u05DD\u05E2\u05D1\u05E8\u05D9\u05EA",
  "\u4ED6\u5011\u7232\u4EC0\u9EBD\u4E0D\u8AAA\u4E2D\u6587"
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
test <- urltools:::unpuny(truth)

stopifnot(identical(input, test))

