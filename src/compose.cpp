#include "compose.h"

bool compose::emptycheck(std::string element){
  if(element.size() == 0){
    return false;
  }
  return true;
}

std::string compose::compose_single(std::string scheme, std::string domain, std::string port, std::string path,
                                    std::string parameter, std::string fragment){
  
  std::string output;
  
  if(emptycheck(scheme)){
    output += scheme + "://";
  }
  
  if(emptycheck(domain)){
    output += domain;
    
    if(emptycheck(port)){
      output += ":" + port;
    } else {
      output += "/";
    }
  }
  
  if(emptycheck(path)){
    output += path;
  }
  
  if(emptycheck(parameter)){
    output += "?";
    output += parameter;
  }
  
  if(emptycheck(fragment)){
    output += "#";
    output += fragment;
  }
  
  return output;
}

std::vector < std::string > compose::compose_multiple(DataFrame parsed_urls){
  
  std::vector < std::string > schemes = parsed_urls["scheme"];
  std::vector < std::string > domains = parsed_urls["domain"];
  std::vector < std::string > ports = parsed_urls["port"];
  std::vector < std::string > paths = parsed_urls["path"];
  std::vector < std::string > parameters = parsed_urls["parameter"];
  std::vector < std::string > fragments = parsed_urls["fragment"];
  
  std::vector < std::string > output(schemes.size());
  
  for(unsigned int i = 0; i < schemes.size(); i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output[i] = compose_single(schemes[i], domains[i], ports[i], paths[i], parameters[i],
                               fragments[i]);
  }
  
  return output;
}
