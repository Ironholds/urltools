#include <Rcpp.h>
using namespace Rcpp;

#ifndef __PARSING_INCLUDED__
#define __PARSING_INCLUDED__
class parsing{
  
  private:
  
    //Lowercase a string
    static std::string str_tolower(std::string x);
    
  public:
    static std::vector < std::string > parse_url(std::string url, bool normalise);
    static std::string extract_parameter(std::string url, std::string parameter);
    static std::string replace_parameter(std::string url, std::string parameter, std::string new_value);
    static std::string extract_host(std::string url);


};
#endif