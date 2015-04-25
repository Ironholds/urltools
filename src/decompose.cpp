#include "decompose.h"

std::string decompose::scheme(std::string& url){
  std::string output;
  std::size_t protocol = url.find("://");
  if((protocol == std::string::npos) | (protocol > 6)){
    //If that's not present, or isn't present at the /beginning/, unknown
    output = "";
  } else {
    output = url.substr(0,protocol);
    url = url.substr((protocol+3));
  }
  return output;
}

std::vector < std::string > decompose::domain_and_port(std::string& url){
  
  std::vector < std::string > output(2);
  std::size_t domain = url.find("/");
  if(domain == std::string::npos){
    output[0] = "";
    output[1] = "";
    return output;
  }
  
  std::string holding = url.substr(0,domain);
  url = url.substr((domain+1));
  std::size_t port = holding.find(":");
  if(port == std::string::npos){
    output[0] = holding;
    output[1] = "";
    return output;
  }
  
  output[0] = holding.substr(0,port);
  output[1] = holding.substr(port+1);
  return output;
}

std::string decompose::path(std::string& url){
  std::string output;
  std::size_t path = url.find("?");
  if(path == std::string::npos){
    std::size_t fragment = url.find("#");
    if(fragment == std::string::npos){
      output = url;
      url = "";
      return output;
    }
    output = url.substr(0,fragment);
    url = url.substr(fragment+1);
    return output;
  }

  output = url.substr(0,path);
  url = url.substr(path+1);
  return output;
}

std::string decompose::query(std::string& url){
  if(url == ""){
    return url;
  }
  
  std::string output;
  std::size_t fragment = url.find("#");
  if(fragment == std::string::npos){
    output = url;
    url = "";
    return output;
  }
  output = url.substr(0,fragment);
  url = url.substr(fragment+1);
  return output;
}