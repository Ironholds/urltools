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
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    output[i] = encoding::internal_url_decode(urls[i]);
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
  
  //For each string..
  for (int i = 0; i < input_size; ++i){
    
    //Extract the protocol. If you can't find it, just encode the entire thing.
    indices = urls[i].find("://");
    if(indices == std::string::npos){
      output[i] = encoding::internal_url_encode(urls[i]);
    } else {
      //Otherwise, split out the protocol and encode !protocol.
      output[i] = urls[i].substr(0,indices+3) + encoding::internal_url_encode(urls[i].substr(indices+3));
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
  
  //Measure size, create output object
  int input_size = urls.size();
  std::list < std::vector < std::string > > output;
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    output.push_back(parsing::parse_url(urls[i], normalise));
  }
  
  //Return
  return output;
}

//'@title extract the value of an API parameter
//'@description \code{extract_parameter} takes a vector of URLs and extracts the value associated with
//'a specified parameter
//'
//'@details People tend to put useful data in URL parameters, particularly around
//'APIs. Extracting these is a pain unless you have a very consistent API, since you're essentially
//'doing partial string-matching with a potentially arbitrary number of characters to include.
//'
//'\code{extract_parameter} accepts a vector of URLs, and the name of the parameter (without an equals sign)
//'and returns the value associated with that parameter. In the case that the parameter
//'is represented multiple times within the URL, the first instance will be used.
//'
//'@param urls a vector of URLs.
//'
//'@param parameter the name of the parameter to search for. Case sensitive (so preprocessing
//'with \code{\link{tolower}} may be useful).
//'
//'@return a character vector containing the value retrieved from each URL.
//'
//'@seealso \code{\link{replace_parameter}} for replacing, rather than extracting, values.
//'@examples
//'url_extract_param(urls = "http://google.org/w/api.php?format=xml&smstate=all", parameter = "format")
//'
//'@export
// [[Rcpp::export]]
std::vector < std::string > extract_parameter(std::vector < std::string > urls, std::string parameter){
  
  //Measure size, create output object
  int input_size = urls.size();
  std::vector < std::string > output(input_size);
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    output[i] = parsing::extract_parameter(urls[i], parameter);
  }
  
  //Return
  return output;
}


//'@title replace the value of an API parameter
//'@description \code{replace_parameter} takes a vector of URLs and replaces the value
//'associated with a particular URL parameter, with one of your choosing.
//'
//'@details As well as component extraction, people can also find it useful to have access
//'to component replacement - for example, mass-modifying a set of URLs used for API queries
//'to output JSON rather than XML.
//'
//'\code{replace_parameter} accepts a vector of URLs, the name of a parameter, and a replacement value.
//'It then loops through the URLs replacing the existing value held by that parameter with the new one.
//'
//'In the case that a parameter is present multiple times in a URL, only the first instance will be amended.
//'In the case that a parameter is not present at all, no change to the URL will be made.
//'
//'@param urls a vector of URLs.
//'
//'@param parameter the name of the parameter to search for. Case sensitive (so preprocessing
//'with \code{\link{tolower}} may be useful).
//'
//'@param new_value the value you want to replace the current value of \code{parameter}.
//'
//'@return a character vector containing the amended URLs. In the situation where the parameter is
//'not present within the URL, the original URL will be returned.
//'
//'@seealso \code{\link{extract_parameter}} for extracting, rather than replacing, values.
//'
//'@examples
//'url_replace_param(urls = "http://google.org/w/api.php?format=xml&smstate=all", parameter = "format", new_value = "json")
//'
//'@export
// [[Rcpp::export]]
std::vector < std::string > replace_parameter(std::vector < std::string > urls, std::string parameter, std::string new_value){
  
  //Measure size, create output object
  int input_size = urls.size();
  std::vector < std::string > output(input_size);
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    output[i] = parsing::replace_parameter(urls[i], parameter, new_value);
  }
  
  //Return
  return output;
}

//'@title extract a URL's hostname and provide it
//'@description similar to \code{\link{url_parse}} or \code{\link{extract_parameter}}, but
//'for hostnames only.
//'
//'@param urls a vector of URLs
//'
//'@return a vector of hostnames. In the event that a hostname cannot be identified, an
//'empty string will be returned.
//'@export
//[[Rcpp::export]]
std::vector < std::string > extract_host(std::vector < std::string > urls){
  
  //Measure size, create output object
  int input_size = urls.size();
  std::vector < std::string > output(input_size);
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    output[i] = parsing::extract_host(urls[i]);
  }
  
  //Return
  return output;
}