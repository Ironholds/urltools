#include <Rcpp.h>
using namespace Rcpp;

#ifndef __COMPOSE_INCLUDED__
#define __COMPOSE_INCLUDED__

/**
 * A namespace for recomposing parsed URLs
 */
namespace compose {
  
  /**
   * A function for briefly checking if a component is empty before doing anything
   * with it
   * 
   * @param str a Rcpp String to check
   * 
   * @return true if the string is not empty, false if it is.
   */
  bool emptycheck(String element);
  
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
   * @return an Rcpp String containing the recomposed URL
   * 
   * @seealso compose_multiple for the vectorised version
   */
  std::string compose_single(String scheme, String domain, String port, String path,
                             String parameter, String fragment);
  
  /**
   * A function for recomposing a vector of URLs
   * 
   * @param parsed_urls a DataFrame provided by url_parse
   * 
   * @return a CharacterVector containing the recomposed URLs
   */
  CharacterVector compose_multiple(DataFrame parsed_urls);
}

#endif
