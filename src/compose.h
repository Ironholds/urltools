#include <Rcpp.h>
using namespace Rcpp;

#ifndef __COMPOSE_INCLUDED__
#define __COMPOSE_INCLUDED__

/**
 * A class for recomposing parsed URLs
 */
class compose {
  
private:
  
  /**
   * A function for briefly checking if a component is empty before doing anything
   * with it
   * 
   * @param str a string to check
   * 
   * @return true if the string is not empty, false if it is.
   */
  bool emptycheck(std::string element);
  
  /**
   * A function for recomposing a single URL
   * 
   * @param scheme the scheme of the URL
   * 
   * @param domain the domain of the URL
   * 
   * @param port the port of the URL
   * 
   * @param path the path of the URL
   * 
   * @param parameter the parameter of the URL
   * 
   * @param fragment the fragment of the URL
   * 
   * @return a string containing the recomposed URL
   */
  std::string compose_single(std::string scheme, std::string domain, std::string port, std::string path,
                             std::string parameter, std::string fragment);
  
public:
  
  /**
   * A function for recomposing a vector of URLs
   * 
   * @param parsed_urls a DataFrame provided by url_parse
   * 
   * @return a string vector containing the recomposed URLs
   */
  std::vector < std::string > compose_multiple(DataFrame parsed_urls);
};

#endif