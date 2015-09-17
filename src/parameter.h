#include "parsing.h"

#ifndef __PARAM_INCLUDED__
#define __PARAM_INCLUDED__


class parameter: public parsing {
  
private:
  
  std::vector < std::string > get_query_string(std::string url);
  
  std::string set_parameter(std::string url, std::string& component, std::string& value);
  
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
  
  std::vector < std::string > set_parameter_vectorised(std::vector < std::string > urls, std::string component,
                                                       std::vector < std::string > value);
  
  std::vector < std::string > remove_parameter_vectorised(std::vector < std::string > urls, std::vector < std::string > params);
    
};

#endif