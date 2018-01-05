#include "parsing.h"

#ifndef __PARAM_INCLUDED__
#define __PARAM_INCLUDED__


namespace parameter {
  
  /**
   * Split out a URL query from the actual body. Used
   * in set_ and remove_parameter.
   * 
   * @param url a URL.
   * 
   * @return a deque either of length 1, indicating that no
   * query was found, or 2, indicating that one was.
   */
  std::deque < std::string > get_query_string(std::string url);
  
  /**
   * Set the value of a single key=value parameter.
   * 
   * @param url a URL.
   * 
   * @param component a reference to the key to set
   * 
   * @param value a reference to the value to set.
   * 
   * @return a string containing URL + key=value, controlling
   * for the possibility that the URL did not previously have a query
   * associated - or did, and /had that key/, but was associating a
   * different value with it.
   */
  std::string set_parameter(std::string url, std::string& component, std::string value);
  
  String get_parameter_single(std::string url, std::string& component);
  /**
   * Reemove a range of key/value parameters
   * 
   * @param url a URL.
   * 
   * @param params a vector of keys.
   * 
   * @return a string containing the URL but absent the keys and values that were specified.
   * 
   */
  std::string remove_parameter_single(std::string url, CharacterVector params);

  std::deque< std::string > get_parameter_names_single(std::string url);
  
  /**
   * Component retrieval specifically for parameters.
   * 
   * @param urls a reference to a vector of URLs
   * 
   * @param component the name of a component to retrieve
   * the value of
   * 
   * @return a vector of the values for that component.
   */
  CharacterVector get_parameter(CharacterVector& urls, std::string component);

  /**
   * Scan a list of URLS for parameter names used.
   *
   * @param A reference to a character vector of urls
   *
   * @return a vector of unique parameter names.
   */
  CharacterVector get_parameter_names(CharacterVector &urls);
  
  /**
   * Set the value of a single key=value parameter for a vector of strings.
   * 
   * @param urls a vector of URLs.
   * 
   * @param component a string containing the key to set
   * 
   * @param value a vector of values to set.
   * 
   * @return the initial URLs vector, with the aforementioned string modifications.
   */
  CharacterVector set_parameter_vectorised(CharacterVector urls, String component,
                                           CharacterVector value);
  
  /**
   * Reemove a range of key/value parameters from a vector of strings.
   * 
   * @param urls a vector of URLs.
   * 
   * @param params a vector of keys.
   * 
   * @return the initial URLs vector, with the aforementioned string modifications.
   * 
   */
  CharacterVector remove_parameter_vectorised(CharacterVector urls,
                                              CharacterVector params);
}

#endif
