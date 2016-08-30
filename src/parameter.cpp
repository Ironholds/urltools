#include "parameter.h"

std::vector < std::string > parameter::get_query_string(std::string url){
  
  std::vector < std::string > output;
  size_t query_location = url.find("?");
  if(query_location == std::string::npos){
    output.push_back(url);
  } else {
    output.push_back(url.substr(0, query_location));
    output.push_back(url.substr(query_location));
  }
  return output;
}


std::string parameter::set_parameter(std::string url, std::string& component, std::string& value){
  
  std::vector < std::string > holding = get_query_string(url);
  if(holding.size() == 1){
    return holding[0] + ("?" + component + "=" + value);
  }
  
  size_t component_location = holding[1].find((component + "="));
  
  if(component_location == std::string::npos){
    holding[1] = (holding[1] + "&" + component + "=" + value);
  } else {
    size_t value_location = holding[1].find("&", component_location);
    if(value_location == std::string::npos){
      holding[1].replace(component_location, value_location, (component + "=" + value));
    } else {
      holding[1].replace(component_location, (value_location - component_location), (component + "=" + value));
    }
    
  }
  
  return(holding[0] + holding[1]);
  
}

std::string parameter::remove_parameter_single(std::string url, std::vector < std::string >& params){
  
  std::vector < std::string > parsed_url = get_query_string(url);
  if(parsed_url.size() == 1){
    return url;
  }
  
  for(unsigned int i = 0; i < params.size(); i++){
    size_t param_location = parsed_url[1].find(params[i]);
    while(param_location != std::string::npos){
      size_t end_location = parsed_url[1].find("&", param_location);
      parsed_url[1].erase(param_location, end_location);
      param_location = parsed_url[i].find(params[i], param_location);
    }
  }
  
  // We may have removed all of the parameters or the last one, leading to trailing ampersands or
  // question marks. If those exist, erase them.
  if(parsed_url[1][parsed_url[1].size()-1] == '&' || parsed_url[1][parsed_url[1].size()-1] == '?'){
    parsed_url[1].erase(parsed_url[1].size()-1);
  }
  
  return (parsed_url[0] + parsed_url[1]);
}

//Parameter retrieval
CharacterVector parameter::get_parameter(CharacterVector& urls, std::string component){
  std::size_t component_location;
  std::size_t next_location;
  unsigned int input_size = urls.size();
  int component_size = component.length();
  CharacterVector output(input_size);
  component = component + "=";
  std::string holding;
  for(unsigned int i = 0; i < input_size; ++i){
    if(urls[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      holding = Rcpp::as<std::string>(urls[i]);
      component_location = holding.find(component);
      if(component_location == std::string::npos){
        output[i] = NA_STRING;
      } else {
        next_location = holding.find_first_of("&#", component_location + component_size);
        if(next_location == std::string::npos){
          output[i] = holding.substr(component_location + component_size + 1);
        } else {
          output[i] = holding.substr(component_location + component_size + 1, (next_location-(component_location + component_size + 1)));
        }
      }
    }
  }
  return output;
}

std::vector < std::string > parameter::set_parameter_vectorised(std::vector < std::string > urls, std::string component,
                                                                std::vector < std::string > value){
  
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

std::vector < std::string > parameter::remove_parameter_vectorised(std::vector < std::string > urls,
                                                                   std::vector < std::string > params){
  
  // Generate easily find-able params.
  for(unsigned int i = 0; i < params.size(); i++){
    params[i] = params[i] + "=";
  }
  std::vector < std::string >& param_ref = params;
  
  // For each URL, remove those parameters.
  for(unsigned int i = 0; i < urls.size(); i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    urls[i] = remove_parameter_single(urls[i], param_ref);
  }
  
  // Return
  return urls;
}