#include "parsing.h"

#ifndef __PARAM_INCLUDED__
#define __PARAM_INCLUDED__


class parameter: public parsing {
  
  
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
};

#endif