##urltools
A package for elegantly handling and parsing URLs from within R.

__Author:__ Oliver Keyes<br/>
__License:__ [MIT](http://opensource.org/licenses/MIT)<br/>
__Status:__ In development

###Description

URLs in R are often treated as nothing more than part of data retrieval -
they're used for making connections and reading data. With web analytics
and research, however, URLs can *be* the data, and R's default handlers
are not best suited to handle vectorised operations over large datasets.
<code>urltools</code> is intended to solve this. 

It contains drop-in replacements for R's URLdecode and URLencode functions, along
with new functionality such as a URL parser and parameter value extractor. In all
cases, the functions are designed to be content-safe (not breaking on unexpected values)
and fully vectorised, resulting in a dramatic speed improvement over existing implementations -
crucial for large datasets. For more information, see the urltools vignette.

###Installation
    library(devtools)
    install_github("ironholds/urltools")

###Dependencies
* R. Doy.
* [Rcpp](http://cran.rstudio.com/web/packages/Rcpp/)

