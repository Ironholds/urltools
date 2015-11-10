#include "parsing.h"

std::string parsing::scheme(std::string& url){
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

std::string parsing::string_tolower(std::string str){
  unsigned int input_size = str.size();
  for(unsigned int i = 0; i < input_size; i++){
    str[i] = tolower(str[i]);
  }
  return str;
}

std::vector < std::string > parsing::domain_and_port(std::string& url){
  
  std::vector < std::string > output(2);
  std::string holding;
  unsigned int output_offset = 0;
  
  // Identify the port. If there is one, push everything
  // before that straight into the output, and the remainder
  // into the holding string. If not, the entire
  // url goes into the holding string.
  std::size_t port = url.find(":");
  
  if(port != std::string::npos && url.find("/") >= port){
    output[0] = url.substr(0,port);
    holding = url.substr(port+1);
    output_offset++;
  } else {
    holding = url;
  }
  
  // Look for a trailing slash
  std::size_t trailing_slash = holding.find("/");
  
  // If there is one, that's when everything ends
  if(trailing_slash != std::string::npos){
    output[output_offset] = holding.substr(0, trailing_slash);
    output_offset++;
    url = holding.substr(trailing_slash+1);
    return output;
  }
  
  // If not, there might be a query parameter associated
  // with the base URL, which we need to preserve.
  std::size_t param = holding.find("?");
  
  // If there is, handle that
  if(param != std::string::npos){
    output[output_offset] = holding.substr(0, param);
    url = holding.substr(param);
    return output;
  }
  
  // Otherwise we're done here
  output[output_offset] = holding;
  url = "";
  return output;
}

std::string parsing::path(std::string& url){
  if(url.size() == 0){
    return url;
  }
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
    url = url.substr(fragment);
    return output;
  }

  output = url.substr(0,path);
  url = url.substr(path+1);
  return output;
}

std::string parsing::query(std::string& url){
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

//URL parser
std::vector < std::string > parsing::url_to_vector(std::string& url){
  
  //Output object, holding object, normalise.
  std::vector < std::string > output(6);
  std::vector < std::string > holding;
  
  std::string s = scheme(url);
  
  holding = domain_and_port(url);
  std::string d = holding[0];
    
  //Run
  output[0] = string_tolower(s);
  output[1] = string_tolower(d);
  output[2] = holding[1];
  output[3] = path(url);
  output[4] = query(url);
  output[5] = url;
  return output;
}

//Component retrieval
std::string parsing::get_component(std::string& url, int component){
  return url_to_vector(url)[component];
}

//Component modification
std::string parsing::set_component(std::string url, int component, std::string new_value){
  std::string url_cp = url;
  std::string parsed_url_elem = url_to_vector(url)[component];
  if(parsed_url_elem.size() != 0){
    url_cp.replace(url_cp.find(parsed_url_elem), parsed_url_elem.size(), new_value);
  }
  return url_cp;
}

DataFrame parsing::parse_to_df(std::vector < std::string >& urls_ptr){
  
  //Input and holding objects
  unsigned int input_size = urls_ptr.size();
  std::vector < std::string > holding(6);
  
  //Output objects
  std::vector < std::string > schemes(input_size);
  std::vector < std::string > domains(input_size);
  std::vector < std::string > ports(input_size);
  std::vector < std::string > paths(input_size);
  std::vector < std::string > parameters(input_size);
  std::vector < std::string > fragments(input_size);
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    holding = url_to_vector(urls_ptr[i]);
    schemes[i] = holding[0];
    domains[i] = holding[1];
    ports[i] = holding[2];
    paths[i] = holding[3];
    parameters[i] = holding[4];
    fragments[i] = holding[5];
  }
  
  return DataFrame::create(_["scheme"] = schemes,
                           _["domain"] = domains,
                           _["port"] = ports,
                           _["path"] = paths,
                           _["parameter"] = parameters,
                           _["fragment"] = fragments,
                           _["stringsAsFactors"] = false);
}