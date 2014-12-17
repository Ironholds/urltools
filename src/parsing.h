#include <Rcpp.h>
using namespace Rcpp;

class parsing{
  
  private:
  
    //Lowercase a string
    static std::string str_tolower(std::string x){
      
      //Create output object
      std::string output;
      int string_size = x.size();
      
      //Loop
      for(int i = 0; i < string_size; i++){
        
        //For each character, lowercase and append
        output += tolower(x[i]);
        
      }
      
      //Return
      return output;
    }
  public:
    static std::vector < std::string > parse_url(std::string url, bool normalise);
    static std::string extract_parameter(std::string url, std::string parameter);
};

//URL parsing
std::vector < std::string > parsing::parse_url(std::string url, bool normalise){
  
  //Output object
  std::vector < std::string > output;
  
  //Normalise, if wanted
  if(normalise){
    url = parsing::str_tolower(url);
  }
  
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

std::string parsing::extract_parameter(std::string url, std::string parameter){
  
  //Output object
  std::string output;
  output = "";

  //Find the parameter. If you can't, return an empty string
  size_t param = url.find(parameter);
  if(param == std::string::npos){
    return output;
  } else if(param + parameter.size() == url.size()) {
    return output;
  } else {
    
    //Substring, find the end of the parameter. If it can't be found, it's the end of the string,
    //so just return the entire thing.
    url = url.substr(param + parameter.size() + 1);
    size_t param_end = url.find("&");
    if(param_end == std::string::npos){
      output = url;
    } else {
      output = url.substr(0, param_end);
    }
  }

  return output;
}