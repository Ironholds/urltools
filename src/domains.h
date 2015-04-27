#include <Rcpp.h>
#include <regex>
using namespace Rcpp;

#ifndef __DOMAIN_MATCHER__
#define __DOMAIN_MATCHER__

class domains {
private:
  std::map<std::regex,std::string> domain_map;
  std::vector < std::string > single_domain_split(std::string& url);
public:
  domains(std::vector < std::string > to_regex, std::vector < std::string > TLD);
  DataFrame domain_split(std::vector < std::string > domains);
};

#endif