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
  std::string holding_1;
  std::string holding_2;
  
  //For each string..
  for (int i = 0; i < input_size; ++i){
    
    //Extract the protocol. If you can't find it, just encode the entire thing.
    indices = urls[i].find("://");
    if(indices == std::string::npos){
      output[i] = encoding::urlencode(urls[i]);
    } else {
      //Otherwise, split out the protocol and encode !protocol.
      holding_1 = urls[i];
      holding_2 = urls[i].substr(0,indices+3);
      holding_1 = holding_1.substr(indices+3);
      holding_1 = encoding::urlencode(holding_1);
      output[i] = holding_2 += holding_1;
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
//'parse_url("https://en.wikipedia.org/wiki/Article")
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
//'@description \code{url_param} takes a vector of URLs and extracts the value associated with
//'a specified parameter
//'
//'@details People tend to put useful data in URL parameters, particularly around
//'APIs. Extracting these is a pain unless you have a very consistent API, since you're essentially
//'doing partial string-matching with a potentially arbitrary number of characters to include.
//'
//'\code{url_param} accepts a vector of URLs, and the name of the parameter (without an equals sign)
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
//'@examples
//'url_param(urls = "https://google.org/w/api.php?action=sitematrix&format=xml&smstate=all", parameter = "format")
//'
//'@export
// [[Rcpp::export]]
std::vector < std::string > url_param(std::vector < std::string > urls, std::string parameter){
  
  //Measure size, create output object
  int input_size = urls.size();
  std::string holding;
  std::vector < std::string > output;
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    output.push_back(parsing::extract_parameter(urls[i], parameter));
  }
  
  //Return
  return output;
}