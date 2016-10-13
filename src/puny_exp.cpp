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
};

String encode_single(std::string x){
  
  clearbuf();
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
        return NA_STRING;
        Rcpp::warning(ret);
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

//[[Rcpp::export]]
CharacterVector encode_vector(CharacterVector x){
  
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