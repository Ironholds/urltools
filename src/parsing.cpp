#include <Rcpp.h>
#include "parsing.h"
using namespace Rcpp;

//Lower case a string
std::string parsing::str_tolower(std::string url){
  
  unsigned int string_size = url.size();
  for(unsigned int i = 0; i < string_size; i++){
    url[i] = tolower(url[i]);
    
  }
  //Return
  return url;
}

//URL parser
std::vector < std::string > parsing::parse_url(std::string url){
  
  //Output object, normalise.
  std::vector < std::string > output;
  url = str_tolower(url);
  
  //Extract hostname field
  std::size_t protocol = url.find("://");
  if((protocol == std::string::npos) | (protocol > 6)){
    //If that's not present, or isn't present at the /beginning/, unknown
    output.push_back("");
  } else {
    output.push_back(url.substr(0,protocol));
    url = url.substr((protocol+3));
  }
  
  //Extract domain, detect if a port is included and extract that if so
  std::size_t domain = url.find("/");
  if(domain == std::string::npos){
    output.push_back("");
    output.push_back("");
  } else {
    std::string holding = url.substr(0,domain);
    url = url.substr((domain+1));
    std::size_t port = holding.find(":");
    if(port == std::string::npos){
      output.push_back(holding);
      output.push_back("");
    } else {
      output.push_back(holding.substr(0,port));
      output.push_back(holding.substr(port+1));
    }
  }
  
  //Extract path
  std::size_t path = url.find("?");
  if(path == std::string::npos){
    output.push_back(url);
    output.push_back("");
    output.push_back("");
    return output;
  } else {
    output.push_back(url.substr(0,path));
    url = url.substr(path+1);
  }
  
  //Query and, finally, fragment
  std::size_t query = url.find("#");
  if(query == std::string::npos){
    output.push_back(url);
    output.push_back("");
    return output;
  } else {
    output.push_back(url.substr(0,query));
    output.push_back(url.substr(query+1));
  }
  
  //Return
  return output;
}

//Component retrieval
std::string parsing::get_component(std::string url, int component){
  return parse_url(url)[component];
}

//Parameter retrieval
std::vector < std::string > parsing::get_parameter(std::vector < std::string >& urls, std::string component){
  std::size_t component_location;
  std::size_t next_location;
  int input_size = urls.size();
  int component_size = component.length();
  std::vector < std::string > output(input_size);
  
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

//Component modification
std::string parsing::set_component(std::string url, int component, std::string new_value){
  std::vector < std::string > parsed_url = parse_url(url);
  if(parsed_url[component] != ""){
    url.replace(url.find(parsed_url[component]),parsed_url[component].size(), new_value);
  }
  return url;
}
