#include <Rcpp.h>
#include "parsing.h"
using namespace Rcpp;

//Lower case a string
std::string parsing::str_tolower(std::string url){
  
  int string_size = url.size();
  for(int i = 0; i < string_size; i++){
    url[i] = tolower(url[i]);
    
  }
  //Return
  return url;
}

//Merge a vector into a string.
std::string parsing::str_merge(std::vector < std::string > parsed_url){
  std::string output;
  std::vector < std::string >::const_iterator it = parsed_url.begin(); 
  for(it; it != parsed_url.end(); ++it){
    output += *it;
  }
  return output;
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

//Component modification
std::string parsing::set_component(std::string url, int component, std::string new_value){
  std::vector < std::string > parsed_url = parse_url(url);
  parsed_url[component] = new_value;
  return str_merge(parsed_url);
}