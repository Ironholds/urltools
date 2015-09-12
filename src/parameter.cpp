#include "parameter.h"

std::string parameter::set_parameter(std::string url, std::string& component, std::string& value){
  
  size_t query_location = url.find("?");
  if(query_location == std::string::npos){
    return url + ("?" + component + "=" + value);
  }
  
  std::string output = url.substr(query_location);
  url = url.substr(0, query_location);
  size_t component_location = output.find((component + "="));
  
  if(component_location == std::string::npos){
    output = (output + "&" + component + "=" + value);
  } else {
    size_t value_location = output.find("&", component_location);
    output.replace(component_location, value_location, (component + "=" + value));
  }
  
  return(url + output);
  
}

//Parameter retrieval
std::vector < std::string > parameter::get_parameter(std::vector < std::string >& urls, std::string component){
  std::size_t component_location;
  std::size_t next_location;
  unsigned int input_size = urls.size();
  int component_size = component.length();
  std::vector < std::string > output(input_size);
  component = component + "=";
  
  for(unsigned int i = 0; i < input_size; ++i){
    component_location = urls[i].find(component);
    if(component_location == std::string::npos){
      output[i] = "";
    } else {
      next_location = urls[i].find_first_of("&#", component_location + component_size);
      if(next_location == std::string::npos){
        output[i] = urls[i].substr(component_location + component_size + 1);
      } else {
        output[i] = urls[i].substr(component_location + component_size + 1, (next_location-(component_location + component_size + 1)));
      }
    }
  }
  return output;
}

std::vector < std::string > parameter::set_parameter_vectorised(std::vector < std::string > urls,
                                                                std::string component, std::vector < std::string > value){
  
  unsigned int input_size = urls.size();
  std::string& component_ref = component;
  if(value.size() == input_size){
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      urls[i] = set_parameter(urls[i], component_ref, value[i]);
    }
  } else if(value.size() == 1){
    std::string& value_ref = value[0];
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      urls[i] = set_parameter(urls[i], component_ref, value_ref);
    }
  } else {
    throw std::range_error("'value' must be the same length as 'urls', or of length 1");
  }
  
  return urls;
}