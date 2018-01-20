#include <Rcpp.h>
#include "encoding.h"
using namespace Rcpp;

char encoding::from_hex (char x){
  if(x <= '9' && x >= '0'){
    x -= '0';
  } else if(x <= 'f' && x >= 'a'){
    x -= ('a' - 10);
  } else if(x <= 'F' && x >= 'A'){
    x -= ('A' - 10);
  } else {
    x = -1;
  }
  return x;
}

std::string encoding::to_hex(char x){
  
  //Holding objects and output
  char digit_1 = (x&0xF0)>>4;
  char digit_2 = (x&0x0F);
  std::string output;
  
  //Convert
  if(0 <= digit_1 && digit_1 <= 9){
    digit_1 += 48;
  } else if(10 <= digit_1 && digit_1 <=15){
    digit_1 += 97-10;
  }
  if(0 <= digit_2 && digit_2 <= 9){
    digit_2 += 48;
  } else if(10 <= digit_2 && digit_2 <= 15){
    digit_2 += 97-10;
  }
  
  output.append(&digit_1, 1);
  output.append(&digit_2, 1);
  return output;
}

std::string encoding::internal_url_decode(std::string url){
  
  //Create output object
  std::string result;

  //For each character...
  for (std::string::size_type i = 0; i <  url.size(); ++i){
    
    //If it's a +, space
    if (url[i] == '+'){
      result += ' ';
    } else if (url[i] == '%' && url.size() > i+2){
      
      //Escaped? Convert from hex and includes
      char holding_1 = encoding::from_hex(url[i+1]);
      char holding_2 = encoding::from_hex(url[i+2]);
      if (holding_1 >= 0 && holding_2 >= 0) {
        char holding = (holding_1 << 4) | holding_2;
        result += holding;
        i += 2;
      } else {
        result += url[i];
      }
      
    } else { //Permitted? Include.
      result += url[i];
    }
  }
  
  //Return
  return result;
}

std::string encoding::internal_url_encode(std::string url){

  //Note the unreserved characters, create an output string
  std::string unreserved_characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._~-";
  std::string output = "";
  
  //For each character..
  for(int i=0; i < (signed) url.length(); i++){
    
    //If it's in the list of reserved ones, just pass it through
    if (unreserved_characters.find_first_of(url[i]) != std::string::npos){
      output.append(&url[i], 1);
    //Otherwise, append in an encoded form.
    } else {
      output.append("%");
      output.append(to_hex(url[i]));
    }
  }
  
  //Return
  return output;
}


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
  
  //Decode each string in turn.
  for (int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = encoding::internal_url_decode(Rcpp::as<std::string>(urls[i]));
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
        output[i] = encoding::internal_url_encode(holding);
      } else {
        //Otherwise, split out the protocol and encode !protocol.
        first_slash = holding.find("/", scheme_start+3);
        if(first_slash == std::string::npos){
          output[i] = holding.substr(0,scheme_start+3) + encoding::internal_url_encode(holding.substr(scheme_start+3));
        } else {
          output[i] = holding.substr(0,first_slash+1) + encoding::internal_url_encode(holding.substr(first_slash+1));
        }
      }
    }
  }
  
  //Return
  return output;
}
