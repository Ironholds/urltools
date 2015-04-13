#include <Rcpp.h>
#include "encoding.h"
#include "parsing.h"
using namespace Rcpp;

//'@title Encode or decode a URI
//'@description encodes or decodes a URI/URL
//'
//'@param urls a vector of URLs to decode or encode.
//'
//'@details
//'URL encoding and decoding is an essential prerequisite to proper web interaction
//'and data analysis around things like server-side logs. The
//'\href{http://tools.ietf.org/html/rfc3986}{relevant IETF RfC} mandates the percentage-encoding
//'of non-Latin characters, including things like slashes, unless those are reserved.
//'
//'Base R provides \code{\link{URLdecode}} and \code{\link{URLencode}}, which handle
//'URL encoding - in theory. In practise, they have a set of substantial problems
//'that the urltools implementation solves::
//'
//'\itemize{
//' \item{No vectorisation: }{Both base R functions operate on single URLs, not vectors of URLs.
//'       This means that, when confronted with a vector of URLs that need encoding or
//'       decoding, your only option is to loop from within R. This can be incredibly
//'       computationally costly with large datasets. url_encode and url_decode are
//'       implemented in C++ and entirely vectorised, allowing for a substantial
//'       performance improvement.}
//' \item{No scheme recognition: }{encoding the slashes in, say, http://, is a good way
//'       of making sure your URL no longer works. Because of this, the only thing
//'       you can encode in URLencode (unless you refuse to encode reserved characters)
//'       is a partial URL, lacking the initial scheme, which requires additional operations
//'       to set up and increases the complexity of encoding or decoding. url_encode
//'       detects the protocol and silently splits it off, leaving it unencoded to ensure
//'       that the resulting URL is valid.}
//' \item{ASCII NULs: }{Server side data can get very messy and sometimes include out-of-range
//'       characters. Unfortunately, URLdecode's response to these characters is to convert
//'       them to NULs, which R can't handle, at which point your URLdecode call breaks.
//'       \code{url_decode} simply ignores them.}
//'}
//'
//'@return a character vector containing the encoded (or decoded) versions of "urls".
//'
//'@examples
//'
//'url_decode("https://en.wikipedia.org/wiki/File:Vice_City_Public_Radio_%28logo%29.jpg")
//'url_encode("https://en.wikipedia.org/wiki/File:Vice_City_Public_Radio_(logo).jpg")
//'
//'\dontrun{
//'#A demonstrator of the contrasting behaviours around out-of-range characters
//'URLdecode("%gIL")
//'url_decode("%gIL")
//'}
//'@rdname encoder
//'@export
// [[Rcpp::export]]
std::vector < std::string > url_decode(std::vector < std::string > urls){
  
  //Measure size, create output object
  int input_size = urls.size();
  std::vector < std::string > output(input_size);
  encoding enc_inst;
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output[i] = enc_inst.internal_url_decode(urls[i]);
  }
  
  //Return
  return output;
}

//'@rdname encoder
//'@export
// [[Rcpp::export]]
std::vector < std::string > url_encode(std::vector < std::string > urls){
  
  //Measure size, create output object and holding objects
  int input_size = urls.size();
  std::vector < std::string > output(input_size);
  size_t indices;
  encoding enc_inst;

  //For each string..
  for (int i = 0; i < input_size; ++i){
    
    //Check for user interrupts.
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    
    //Extract the protocol. If you can't find it, just encode the entire thing.
    indices = urls[i].find("://");
    if(indices == std::string::npos){
      output[i] = enc_inst.internal_url_encode(urls[i]);
    } else {
      //Otherwise, split out the protocol and encode !protocol.
      output[i] = urls[i].substr(0,indices+3) + enc_inst.internal_url_encode(urls[i].substr(indices+3));
    }
  }
  
  //Return
  return output;
}

//'@title split URLs into their component parts
//'@description \code{url_parse} takes a vector of URLs and splits each one into its component
//'parts, as recognised by RfC 3986.
//'
//'@param urls a vector of URLs
//'
//'@param normalise whether to normalise the URLs - essentially, whether or not to
//'make them consistently lower-case. Set to TRUE by default.
//'
//'@details It's useful to be able to take a URL and split it out into its component parts - 
//'for the purpose of hostname extraction, for example, or analysing API calls. This functionality
//'is not provided in base R, although it is provided in \code{\link[httr]{parse_url}}; that
//'implementation is entirely in R, uses regular expressions, and is not vectorised. It's
//'perfectly suitable for the intended purpose (decomposition in the context of automated
//'HTTP requests from R), but not for large-scale analysis.
//'
//'@return a list of vectors, one for each URL, with each vector containing (in sequence)
//'the URL's scheme, domain, port, path, query string and fragment identifier. See the
//'\href{http://tools.ietf.org/html/rfc3986}{relevant IETF RfC} for definitions. If an element
//'cannot be identified, it is replaced with an empty string.
//'
//'@examples
//'url_parse("https://en.wikipedia.org/wiki/Article")
//'
//'@export
// [[Rcpp::export]]
std::list < std::vector < std::string > > url_parse(std::vector < std::string > urls, bool normalise = true){
  
  //Measure size, create output object, instantiate
  unsigned int input_size = urls.size();
  std::list < std::vector < std::string > > output;
  parsing p_inst;
  
  //Parse each string in turn.
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output.push_back(p_inst.parse_url(urls[i]));
  }
  
  //Return
  return output;
}

//[[Rcpp::export]]
std::vector < std::string > v_get_component(std::vector < std::string > urls, int component){
  parsing p_inst;
  unsigned int input_size = urls.size();
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    urls[i] = p_inst.get_component(urls[i], component);
  }
  return urls;
}

//'@title get the values of a URL's parameters
//'@description URLs can have parameters, taking the form of \code{name=value}, chained together
//'with \code{&} symbols. \code{url_parameters}, when provided with a vector of URLs and a vector
//'of parameter names, will generate a data.frame consisting of the values of each parameter
//'for each URL.
//'
//'@param urls a vector of URLs
//'
//'@param parameter_names a vector of parameter names
//'
//'@examples
//'#A very simple example
//'url <- "https://www.google.com:80/foo.php?api_params=parsable&this_parameter=selfreferencing&hiphop=awesome"
//'parameter_values <- url_parameters(url, c("api_params","hiphop"))
//'
//'@export
//[[Rcpp::export]]
List url_parameters(std::vector < std::string > urls, std::vector < std::string > parameter_names){
  parsing p_inst;
  List output;
  IntegerVector rownames = Rcpp::seq(1,urls.size());
  int column_count = parameter_names.size();
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

//[[Rcpp::export]]
std::vector < std::string > v_set_component(std::vector < std::string > urls, int component, std::string new_value){
  parsing p_inst;
  unsigned int input_size = urls.size();
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    urls[i] = p_inst.set_component(urls[i], component, new_value);
  }
  return urls;
}