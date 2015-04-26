#include <Rcpp.h>
#include "parsing.h"
using namespace Rcpp;

//[[Rcpp::export]]
std::vector < std::string > get_component_(std::vector < std::string > urls, int component){
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

//[[Rcpp::export]]
std::vector < std::string > set_component_(std::vector < std::string > urls, int component,
                                           std::string new_value){
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