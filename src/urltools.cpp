#include <Rcpp.h>
#include "encoding.h"
#include "compose.h"
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
//'@seealso \code{\link{puny_decode}} and \code{\link{puny_encode}}, for punycode decoding
//'and encoding.
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
CharacterVector url_decode(CharacterVector urls){
  
  //Measure size, create output object
  int input_size = urls.size();
  CharacterVector output(input_size);
  encoding enc_inst;
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = enc_inst.internal_url_decode(Rcpp::as<std::string>(urls[i]));
    }
  }
  
  //Return
  return output;
}

//'@rdname encoder
//'@export
// [[Rcpp::export]]
CharacterVector url_encode(CharacterVector urls){
  
  //Measure size, create output object and holding objects
  int input_size = urls.size();
  CharacterVector output(input_size);
  std::string holding;
  size_t scheme_start;
  size_t first_slash;
  encoding enc_inst;

  //For each string..
  for (int i = 0; i < input_size; ++i){
    
    //Check for user interrupts.
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    
    if(urls[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      holding = Rcpp::as<std::string>(urls[i]);

      //Extract the protocol. If you can't find it, just encode the entire thing.
      scheme_start = holding.find("://");
      if(scheme_start == std::string::npos){
        output[i] = enc_inst.internal_url_encode(holding);
      } else {
        //Otherwise, split out the protocol and encode !protocol.
        first_slash = holding.find("/", scheme_start+3);
        if(first_slash == std::string::npos){
          output[i] = holding.substr(0,scheme_start+3) + enc_inst.internal_url_encode(holding.substr(scheme_start+3));
        } else {
          output[i] = holding.substr(0,first_slash+1) + enc_inst.internal_url_encode(holding.substr(first_slash+1));
        }
      }
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
//'@details It's useful to be able to take a URL and split it out into its component parts - 
//'for the purpose of hostname extraction, for example, or analysing API calls. This functionality
//'is not provided in base R, although it is provided in \code{\link[httr]{parse_url}}; that
//'implementation is entirely in R, uses regular expressions, and is not vectorised. It's
//'perfectly suitable for the intended purpose (decomposition in the context of automated
//'HTTP requests from R), but not for large-scale analysis.
//'
//'Note that user authentication/identification information is not extracted;
//'this can be found with \code{\link{get_credentials}}.
//'
//'@return a data.frame consisting of the columns scheme, domain, port, path, query
//'and fragment. See the '\href{http://tools.ietf.org/html/rfc3986}{relevant IETF RfC} for
//'definitions. If an element cannot be identified, it is represented by an empty string.
//'
//'@examples
//'url_parse("https://en.wikipedia.org/wiki/Article")
//'
//'@seealso \code{\link{url_parameters}} for extracting values associated with particular keys in a URL's
//'query string, and \code{\link{url_compose}}, which is \code{url_parse} in reverse.
//'
//'@export
//[[Rcpp::export]]
DataFrame url_parse(CharacterVector urls){
  CharacterVector& urls_ptr = urls;
  parsing p_inst;
  return p_inst.parse_to_df(urls_ptr);
}

//'@title Recompose Parsed URLs
//'
//'@description Sometimes you want to take a vector of URLs, parse them, perform
//'some operations and then rebuild them. \code{url_compose} takes a data.frame produced
//'by \code{\link{url_parse}} and rebuilds it into a vector of full URLs (or: URLs as full
//'as the vector initially thrown into url_parse).
//'
//'This is currently a `beta` feature; please do report bugs if you find them.
//'
//'@param parsed_urls a data.frame sourced from \code{\link{url_parse}}
//'
//'@seealso \code{\link{scheme}} and other accessors, which you may want to
//'run URLs through before composing them to modify individual values.
//'
//'@examples
//'#Parse a URL and compose it
//'url <- "http://en.wikipedia.org"
//'url_compose(url_parse(url))
//'
//'@export
//[[Rcpp::export]]
CharacterVector url_compose(DataFrame parsed_urls){
  compose c_inst;
  return c_inst.compose_multiple(parsed_urls);
}
