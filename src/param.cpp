#include <Rcpp.h>
#include "parameter.h"
using namespace Rcpp;


//'@title get the values of a URL's parameters
//'@description URLs can have parameters, taking the form of \code{name=value}, chained together
//'with \code{&} symbols. \code{param_get}, when provided with a vector of URLs and a vector
//'of parameter names, will generate a data.frame consisting of the values of each parameter
//'for each URL.
//'
//'@param urls a vector of URLs
//'
//'@param parameter_names a vector of parameter names
//'
//'@return a data.frame containing one column for each provided parameter name. Values that
//'cannot be found within a particular URL are represented by an empty string.
//'
//'@examples
//'#A very simple example
//'url <- "https://google.com:80/foo.php?this_parameter=selfreferencing&hiphop=awesome"
//'parameter_values <- param_get(url, c("this_parameter","hiphop"))
//'
//'@seealso \code{\link{url_parse}} for decomposing URLs into their constituent parts.
//'
//'@export
//[[Rcpp::export]]
List param_get(std::vector < std::string > urls, std::vector < std::string > parameter_names){
  parameter p_inst;
  List output;
  IntegerVector rownames = Rcpp::seq(1,urls.size());
  unsigned int column_count = parameter_names.size();
  std::vector < std::string >&url_ref = urls;
  
  for(unsigned int i = 0; i < column_count; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output.push_back(p_inst.get_parameter(url_ref, parameter_names[i]));
  }
  output.attr("class") = "data.frame";
  output.attr("names") = parameter_names;
  output.attr("row.names") = rownames;
  return output;
}
