#include "parsing.h"

#ifndef __PARAM_INCLUDED__
#define __PARAM_INCLUDED__


class parameter: public parsing {
  
private:
  
  /**
   * Split out a URL query from the actual body. Used
   * in set_ and remove_parameter.
   * 
   * @param url a URL.
   * 
   * @return a vector either of length 1, indicating that no
   * query was found, or 2, indicating that one was.
   */
  std::vector < std::string > get_query_string(std::string url);
  
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
  std::string set_parameter(std::string url, std::string& component, std::string& value);
  
  /**
   * Reemove a range of key/value parameters
   * 
   * @param url a URL.
   * 
   * @param params a reference to a vector of keys.
   * 
   * @return a string containing the URL but absent the keys and values that were specified.
   * 
   */
  std::string remove_parameter_single(std::string url, std::vector < std::string >& params);
  
public:
  
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
  std::vector < std::string > get_parameter(std::vector < std::string >& urls, std::string component);
  
  
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
  std::vector < std::string > set_parameter_vectorised(std::vector < std::string > urls, std::string component,
                                                       std::vector < std::string > value);
  
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
  std::vector < std::string > remove_parameter_vectorised(std::vector < std::string > urls, std::vector < std::string > params);
};

#endif