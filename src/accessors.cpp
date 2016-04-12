#include <Rcpp.h>
#include "parsing.h"
using namespace Rcpp;

//[[Rcpp::export]]
CharacterVector get_component_(CharacterVector urls, int component){
  parsing p_inst;
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] != NA_STRING){
      output[i] = p_inst.get_component(Rcpp::as<std::string>(urls[i]), component);
    } else {
      output[i] = NA_STRING;
    }
  }
  return output;
}

//[[Rcpp::export]]
CharacterVector set_component_(CharacterVector urls, int component,
                               String new_value){
  parsing p_inst;
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    
    output[i] = p_inst.set_component(Rcpp::as<std::string>(urls[i]), component, new_value);
  }
  return output;
}