## urltools
A package for elegantly handling and parsing URLs from within R.

__Author:__ Oliver Keyes, Jay Jacobs<br/>
__License:__ [MIT](http://opensource.org/licenses/MIT)<br/>
__Status:__ Stable

[![Travis-CI Build Status](https://travis-ci.org/Ironholds/urltools.svg?branch=master)](https://travis-ci.org/Ironholds/urltools) ![downloads](http://cranlogs.r-pkg.org/badges/grand-total/urltools)

### Description

URLs in R are often treated as nothing more than part of data retrieval -
they're used for making connections and reading data. With web analytics
and research, however, URLs can *be* the data, and R's default handlers
are not best suited to handle vectorised operations over large datasets.
<code>urltools</code> is intended to solve this. 

It contains drop-in replacements for R's URLdecode and URLencode functions, along
with new functionality such as a URL parser and parameter value extractor. In all
cases, the functions are designed to be content-safe (not breaking on unexpected values)
and fully vectorised, resulting in a dramatic speed improvement over existing implementations -
crucial for large datasets. For more information, see the [urltools vignette](https://github.com/Ironholds/urltools/blob/master/vignettes/urltools.Rmd).

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md).
By participating in this project you agree to abide by its terms.

### Installation

The latest CRAN version can be obtained via:

    install.packages("urltools")
    
To get the development version:

    devtools::install_github("ironholds/urltools")

### Dependencies
* R. Doy.
* [Rcpp](https://cran.r-project.org/package=Rcpp)