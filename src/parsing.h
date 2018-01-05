#include <Rcpp.h>
using namespace Rcpp;

#ifndef __PARSING_INCLUDED__
#define __PARSING_INCLUDED__

namespace parsing {
  
  /**
   * A function for parsing a URL and turning it into a vector.
   * Tremendously useful (read: everything breaks without this)
   * 
   * @param url a URL.
   * 
   * @see get_ and set_component, which call this.
   * 
   * @return a vector consisting of the value for each component
   * part of the URL.
   */
  CharacterVector url_to_vector(std::string url);
  
  /**
   * A function for lower-casing an entire string
   * 
   * @param str a string to lower-case
   * 
   * @return a string containing the lower-cased version of the
   * input.
   */
  std::string string_tolower(std::string str);
    
  /**
   * A function for extracting the scheme of a URL; part of the
   * URL parsing framework.
   * 
   * @param url a reference to a url.
   * 
   * @see url_to_vector which calls this.
   * 
   * @return a string containing the scheme of the URL if identifiable,
   * and "" if not.
   */
  std::string scheme(std::string& url);
  
  /**
   * A function for extracting the domain and port of a URL; part of the
   * URL parsing framework. Fairly unique in that it outputs a
   * vector, unlike the rest of the framework, which outputs a string,
   * since it has to handle multiple elements.
   * 
   * @param url a reference to a url. Should've been run through
   * scheme() first.
   * 
   * @see url_to_vector which calls this.
   * 
   * @return a vector containing the domain and port of the URL if identifiable,
   * and "" for each non-identifiable element.
   */
  std::vector < std::string > domain_and_port(std::string& url);
  
  /**
   * A function for extracting the path of a URL; part of the
   * URL parsing framework.
   * 
   * @param url a reference to a url. Should've been run through
   * scheme() and domain_and_port() first.
   * 
   * @see url_to_vector which calls this.
   * 
   * @return a string containing the path of the URL if identifiable,
   * and "" if not.
   */
  std::string path(std::string& url);
  
  /**
   * A function for extracting the path of a URL; part of the
   * URL parsing framework.
   * 
   * @param url a reference to a url. Should've been run through
   * scheme(), domain_and_port() and path() first.
   * 
   * @see url_to_vector which calls this.
   * 
   * @return a string containing the query string of the URL if identifiable,
   * and "" if not.
   */
  std::string query(std::string& url);
  
  String check_parse_out(std::string x);
    
  /**
   * A function to retrieve an individual component from a parsed
   * URL. Used in scheme(), host() et al; calls parse_url.
   * 
   * @param url a URL.
   * 
   * @param component an integer representing which value in
   * parse_url's returned vector to grab.
   * 
   * @see set_component, which allows for modification.
   * 
   * @return a string consisting of the requested URL component.
   */
  String get_component(std::string url, int component);
  
  /**
   * A function to set an individual component in a parsed
   * URL. Used in "scheme<-", et al; calls parse_url.
   * 
   * @param url a URL.
   * 
   * @param component an integer representing which value in
   * parse_url's returned vector to modify.
   * 
   * @param new_value the value to insert into url[component].
   * 
   * @param rm whether the intent is to remove the component
   * (in which case new_value must be an NA_STRING)
   * 
   * @see get_component, which allows for retrieval.
   * 
   * @return a string consisting of the modified URL.
   */
  String set_component(std::string url, int component, String new_value,
                       bool rm);
  
  /**
   * Decompose a vector of URLs and turn it into a data.frame.
   * 
   * @param URLs a reference to a vector of URLs
   * 
   * @return an Rcpp data.frame.
   * 
   */
  DataFrame parse_to_df(CharacterVector& urls_ptr);
}

#endif
