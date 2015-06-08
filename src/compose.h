#include <Rcpp.h>
using namespace Rcpp;

#ifndef __COMPOSE_INCLUDED__
#define __COMPOSE_INCLUDED__

class compose {
  
private:
  
  bool emptycheck(std::string element);
  
  std::string compose_single(std::string scheme, std::string domain, std::string port, std::string path,
                             std::string parameter, std::string fragment);
  
public:
  
  std::vector < std::string > compose_multiple(DataFrame parsed_urls);
};

#endif