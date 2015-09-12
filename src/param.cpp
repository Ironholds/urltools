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
//'@seealso \code{\link{url_parse}} for decomposing URLs into their constituent parts and
//'\code{\link{param_set}} for inserting or modifying key/value pairs within a query string.
//'
//'@aliases param_get url_parameter
//'@rdname param_get
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

//'@title Set the value associated with a parameter in a URL's query.
//'@description URLs often have queries associated with them, particularly URLs for
//'APIs, that look like \code{?key=value&key=value&key=value}. \code{param_set}
//'allows you to modify key/value pairs within query strings, or even add new ones
//'if they don't exist within the URL.
//'
//'@param urls a vector of URLs. These should be decoded (with \code{url_decode})
//'but do not have to have been otherwise manipulated.
//'
//'@param key a string representing the key to modify the value of (or insert wholesale
//'if it doesn't exist within the URL).
//'
//'@param value a value to associate with the key. This can be a single string,
//'or a vector the same length as \code{urls}
//'
//'@return the original vector of URLs, but with modified/inserted key-value pairs.
//'
//'@examples
//'# Set a URL parameter where there's already a key for that
//'param_set("https://en.wikipedia.org/api.php?action=query", "action", "pageinfo")
//'
//'# Set a URL parameter where there isn't.
//'param_set("https://en.wikipedia.org/api.php?list=props", "action", "pageinfo")
//'
//'@seealso \code{\link{param_get}} to retrieve the values associated with multiple keys in
//'a vector of URLs, and \code{\link{param_remove}} to strip key/value pairs from a URL entirely.
//'
//'@export
//[[Rcpp::export]]
std::vector < std::string > param_set(std::vector < std::string > urls, std::string key, std::vector < std::string > value){
  parameter p_inst;
  return p_inst.set_parameter_vectorised(urls, key, value);
}

//'@title Remove key-value pairs from query strings
//'@description URLs often have queries associated with them, particularly URLs for
//'APIs, that look like \code{?key=value&key=value&key=value}. \code{param_remove}
//'allows you to remove key/value pairs while leaving the rest of the URL intact.
//'
//'@param urls a vector of URLs. These should be decoded with \code{url_decode} but don't
//'have to have been otherwise processed.
//'
//'@param keys a vector of parameter keys to remove.
//'
//'@return the original URLs but with the key/value pairs specified by \code{keys} removed.
//'
//'@seealso \code{\link{param_set}} to modify values associated with keys, or \code{\link{param_get}}
//'to retrieve those values.
//'
//'@examples
//'# Remove multiple parameters from a URL
//'param_remove(urls = "https://en.wikipedia.org/wiki/api.php?action=list&type=query&format=json",
//'             keys = c("action","format"))
//'@export
//[[Rcpp::export]]
std::vector < std::string > param_remove(std::vector < std::string > urls, std::vector < std::string > keys){
  parameter p_inst;
  return p_inst.remove_parameter_vectorised(urls, keys);
  
}

std::vector < std::string > param_compress(std::vector < std::string > urls, bool strict = true){
  return urls;
  
}