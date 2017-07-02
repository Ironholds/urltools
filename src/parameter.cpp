#include "parameter.h"

std::deque < std::string > parameter::get_query_string(std::string url){
  
  std::deque < std::string > output;
  size_t query_location = url.find("?");
  if(query_location == std::string::npos){
    output.push_back(url);
  } else {
    output.push_back(url.substr(0, query_location));
    output.push_back(url.substr(query_location));
  }
  return output;
}

std::string parameter::set_parameter(std::string url, std::string& component, std::string value){
  
  std::deque < std::string > holding = get_query_string(url);
  if(holding.size() == 1){
    return holding[0] + ("?" + component + "=" + value);
  }
  
  size_t component_location = std::string::npos, q_loc, amp_loc;
  q_loc = holding[1].find(("?" + component + "="));
  if(q_loc == std::string::npos){
    amp_loc = holding[1].find(("&" + component + "="));
    if(amp_loc != std::string::npos){
      component_location = amp_loc + 1;
    }
  } else {
    component_location = q_loc + 1;
  }
  
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

std::string parameter::remove_parameter_single(std::string url, CharacterVector params){
  
  std::deque < std::string > parsed_url = get_query_string(url);
  if(parsed_url.size() == 1){
    return url;
  }
  
  for(unsigned int i = 0; i < params.size(); i++){
    if(params[i] != NA_STRING){
      size_t param_location = parsed_url[1].find(Rcpp::as<std::string>(params[i]));
      while(param_location != std::string::npos){
        size_t end_location = parsed_url[1].find("&", param_location);
        parsed_url[1].erase(param_location, end_location);
        param_location = parsed_url[i].find(params[i], param_location);
      }
    }
  }
  
  // We may have removed all of the parameters or the last one, leading to trailing ampersands or
  // question marks. If those exist, erase them.
  if(parsed_url[1][parsed_url[1].size()-1] == '&' || parsed_url[1][parsed_url[1].size()-1] == '?'){
    parsed_url[1].erase(parsed_url[1].size()-1);
  }
  
  return (parsed_url[0] + parsed_url[1]);
}

String parameter::get_parameter_single(std::string url, std::string& component){
  
  // Extract actual query string
  std::deque < std::string > parsed_entry = get_query_string(url);
  if(parsed_entry.size() < 2){
    return NA_STRING;
  }
  std::string holding = parsed_entry[1];
  int component_size;
  
  // ID where the location is
  size_t first_find = holding.find(component);
  if(first_find == std::string::npos){
    return NA_STRING;
  }
  if(holding[first_find-1] != '&' & holding[first_find-1] != '?'){
    first_find = holding.find("&" + component);
    component_size = (component.size() + 1);
    if(first_find == std::string::npos){
      return NA_STRING;
    }
  } else {
    component_size = component.size();
  }
  
  
  size_t next_location = holding.find_first_of("&#", first_find + component_size);
  if(next_location == std::string::npos){
    return holding.substr(first_find + component_size);
  }
  return holding.substr(first_find + component_size, (next_location-(first_find + component_size)));
  
}

//Parameter retrieval
CharacterVector parameter::get_parameter(CharacterVector& urls, std::string component){
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  component = component + "=";
  
  for(unsigned int i = 0; i < input_size; ++i){
    if(urls[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = get_parameter_single(Rcpp::as<std::string>(urls[i]), component);
    }
  }
  return output;
}

CharacterVector parameter::set_parameter_vectorised(CharacterVector urls, String component,
                                                    CharacterVector value){
  
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  
  if(component != NA_STRING){
    std::string component_ref = component.get_cstring();
    if(value.size() == input_size){
      for(unsigned int i = 0; i < input_size; i++){
        if((i % 10000) == 0){
          Rcpp::checkUserInterrupt();
        }
        if(urls[i] != NA_STRING && value[i] != NA_STRING){
          output[i] = set_parameter(Rcpp::as<std::string>(urls[i]), component_ref,
                                    Rcpp::as<std::string>(value[i]));
        } else if(value[i] == NA_STRING){
          output[i] = urls[i];
        } else {
          output[i] = NA_STRING;
        }
      }
    } else if(value.size() == 1){
      if(value[0] != NA_STRING){
        std::string value_ref = Rcpp::as<std::string>(value[0]);
        for(unsigned int i = 0; i < input_size; i++){
          if((i % 10000) == 0){
            Rcpp::checkUserInterrupt();
          }
          if(urls[i] != NA_STRING){
            output[i] = set_parameter(Rcpp::as<std::string>(urls[i]), component_ref, value_ref);
          } else {
            output[i] = NA_STRING;
          }
        }
      } else {
        return urls;
      }
      
    } else {
      throw std::range_error("'value' must be the same length as 'urls', or of length 1");
    }
  } else {
    return urls;
  }

  return output;
}

CharacterVector parameter::remove_parameter_vectorised(CharacterVector urls,
                                                       CharacterVector params){
  
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  CharacterVector p_copy = params;
  // Generate easily find-able params.
  for(unsigned int i = 0; i < p_copy.size(); i++){
    if(p_copy[i] != NA_STRING){
      p_copy[i] += "=";
    }
  }

  // For each URL, remove those parameters.
  for(unsigned int i = 0; i < urls.size(); i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] != NA_STRING){
      output[i] = remove_parameter_single(Rcpp::as<std::string>(urls[i]), p_copy);
      
    } else {
      output[i] = NA_STRING;
    }
  }
  
  // Return
  return output;
}