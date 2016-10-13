#include <Rcpp.h>
#include "punycode.h"
extern "C"{
#include "utf8.h"
}
using namespace Rcpp;

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

String encode_single(std::string x){
  
  url holding;
  Rcpp::warning(("."));
  split_url(x, holding);
  std::string output = holding.path;
  
  for(unsigned int i = 0; i < holding.split_url.size(); i++){
    Rcpp::warning(("."));
    if(holding.split_url[i].find_first_not_of(ascii) == std::string::npos){
      output += holding.split_url[i] + ".";
    } else {
      punycode_uint buflen;
      punycode_uint unilen = BUFLENT;
      const char *s = holding.split_url[i].c_str();
      const int slen = strlen(s);
      Rcpp::warning(("."));
      punycode_decode(slen, s, &unilen, ibuf, NULL);
      buflen = u8_toutf8(buf, BUFLENT, ibuf, unilen);
      output += std::string(reinterpret_cast<char*>(buf), buflen) + ".";
      Rcpp::warning(output);
    }

  }
  Rcpp::warning(("."));
  output.erase(output.size()-1,1);
  Rcpp::warning(("."));
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