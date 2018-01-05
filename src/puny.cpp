#include <Rcpp.h>
#include "punycode.h"
extern "C"{
#include "utf8.h"
}
using namespace Rcpp;
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

#define BUFLENT 2048
static char buf[BUFLENT];
static uint32_t ibuf[BUFLENT];
static std::string ascii = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.?&=:/";

static inline void clearbuf(){
  for (int i=0; i<BUFLENT; i++){
    buf[i] = '\0';
    ibuf[i] = 0;
  }
}

struct url {
  std::deque<std::string> split_url;
  std::string protocol;
  std::string path;
};

void split_url(std::string x, url& output){

  size_t last;
  size_t loc = x.find(".");
  
  last = x.find("://");
  if(last != std::string::npos){
    output.protocol = x.substr(0, (last + 3));
    x = x.substr(last + 3);
  }
  last = x.find_first_of(":/");
  if(last != std::string::npos){
    output.path = x.substr(last);
    x = x.substr(0, last);
  }

  last = 0;
  loc = x.find(".");
  while (loc != std::string::npos) {
    output.split_url.push_back(x.substr(last, loc-last));
    last = ++loc;
    loc = x.find(".", loc);
  }
  if (loc == std::string::npos){
    output.split_url.push_back(x.substr(last, x.length()));
  }
}

std::string check_result(enum punycode_status& st, std::string& x){
  std::string ret = "Error with the URL " + x + ":";
  if (st == punycode_bad_input){
    ret += "input is invalid";
  } else if (st == punycode_big_output){
    ret += "output would exceed the space provided";
  } else if (st == punycode_overflow){
    ret += "input needs wider integers to process";
  } else {
    return "";
  }
  return ret;
}

String encode_single(std::string x){
  
  url holding;
  split_url(x, holding);
  std::string output = holding.protocol;
  
  for(unsigned int i = 0; i < holding.split_url.size(); i++){
    // Check if it's ASCII-only fragment - if so, nowt to do here.
    if(holding.split_url[i].find_first_not_of(ascii) == std::string::npos){
      output += holding.split_url[i];
      if(i < (holding.split_url.size() - 1)){
        output += ".";
      }
    } else {
      
      // Prep for conversion
      punycode_uint buflen = BUFLENT;
      punycode_uint unilen = BUFLENT;
      const char *s = holding.split_url[i].c_str();
      const int slen = strlen(s);
      
      // Do the conversion
      unilen = u8_toucs(ibuf, unilen, s, slen);
      enum punycode_status st = punycode_encode(unilen, ibuf, NULL, &buflen, buf);
      
      // Check it worked
      std::string ret = check_result(st, x);
      if(ret.size()){
        Rcpp::warning(ret);
        return NA_STRING;
      }
      
      std::string encoded = Rcpp::as<std::string>(Rf_mkCharLenCE(buf, buflen, CE_UTF8));
      if(encoded != holding.split_url[i]){
        encoded = "xn--" + encoded;
      }
      output += encoded;
      if(i < (holding.split_url.size() - 1)){
        output += ".";
      }
    }
  }
  output += holding.path;
  return output;
}

//'@title Encode or Decode Internationalised Domains
//'@description \code{puny_encode} and \code{puny_decode} implement
//'the encoding standard for internationalised (non-ASCII) domains and
//'subdomains. You can use them to encode UTF-8 domain names, or decode
//'encoded names (which start "xn--"), or both.
//'
//'@param x a vector of URLs. These should be URL decoded using \code{\link{url_decode}}.
//'
//'@return a CharacterVector containing encoded or decoded versions of the entries in \code{x}.
//'Invalid URLs (ones that are \code{NA}, or ones that do not successfully map to an actual
//'decoded or encoded version) will be returned as \code{NA}.
//'
//'@examples
//'# Encode a URL
//'puny_encode("https://www.bücher.com/foo")
//'
//'# Decode the result, back to the original
//'puny_decode("https://www.xn--bcher-kva.com/foo")
//'
//'@seealso \code{\link{url_decode}} and \code{\link{url_encode}} for percent-encoding.
//'
//'@rdname puny
//'@export
//[[Rcpp::export]]
CharacterVector puny_encode(CharacterVector x){
  
  unsigned int input_size = x.size();
  CharacterVector output(input_size);
  
  for(unsigned int i = 0; i < input_size; i++){
    
    if(i % 10000 == 0){
      Rcpp::checkUserInterrupt();
    }
    
    if(x[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = encode_single(Rcpp::as<std::string>(x[i]));
    }
  }
  
  clearbuf();
  return output;
}

String decode_single(std::string x){
  url holding;
  split_url(x, holding);
  String output(holding.protocol, CE_UTF8);
  
  for(unsigned int i = 0; i < holding.split_url.size(); i++){
    // Check if it's ASCII-only fragment - if so, nowt to do here.
    if(holding.split_url[i].size() < 4 || holding.split_url[i].substr(0,4) != "xn--"){
      output += holding.split_url[i];
      if(i < (holding.split_url.size() - 1)){
        output += ".";
      }
    } else {
      
      // Prep for conversion
      punycode_uint unilen = BUFLENT;
      std::string tmp = holding.split_url[i].substr(4);
      const char *s = tmp.c_str();
      const int slen = strlen(s);
      
      // Do the conversion
      enum punycode_status st = punycode_decode(slen, s, &unilen, ibuf, NULL);
      
      // Check it worked
      std::string ret = check_result(st, x);
      if(ret.size()){
        Rcpp::warning(ret);
        return NA_STRING;
      }
      u8_toutf8(buf, BUFLENT, ibuf, unilen);
      output += buf;
      if(i < (holding.split_url.size() - 1)){
        output += ".";
      }
    }
  }
  output += holding.path;
  return output;
}

//'@rdname puny
//'@export
//[[Rcpp::export]]
CharacterVector puny_decode(CharacterVector x){
  
  unsigned int input_size = x.size();
  CharacterVector output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    
    if(i % 10000 == 0){
      Rcpp::checkUserInterrupt();
    }
    
    if(x[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = decode_single(Rcpp::as<std::string>(x[i]));
    }
  }
  
  return output;
}
