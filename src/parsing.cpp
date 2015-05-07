#include "parsing.h"

//Lower case a string
std::string parsing::str_tolower(std::string& url){
  
  unsigned int string_size = url.size();
  for(unsigned int i = 0; i < string_size; i++){
    url[i] = tolower(url[i]);
  }
  return url;
  
}

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

std::vector < std::string > parsing::domain_and_port(std::string& url){
  
  std::vector < std::string > output(2);
  std::size_t domain = url.find("/");
  if(domain == std::string::npos){
    output[0] = url;
    output[1] = "";
    url = "";
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

std::string parsing::path(std::string& url){
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
  url = str_tolower(url);
  
  //Run
  output[0] = scheme(url);
  holding = domain_and_port(url);
  output[1] = holding[0];
  output[2] = holding[1];
  output[3] = path(url);
  output[4] = query(url);
  output[5] = url;
  return output;
}

std::vector < std::string > parsing::identify_single_suffix(std::string& domain_ptr, std::vector < std::string >& suffix_ptr){
  
}

//Parameter retrieval
std::vector < std::string > parsing::get_parameter(std::vector < std::string >& urls, std::string component){
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

DataFrame parsing::identify_multi_suffix(std::vector < std::string >& domains_ptr, std::vector < std::string >& suffix_ptr){
  
  int input_size = domains_ptr.size();
  std::vector < std::string > domain_fragment(input_size);
  std::vector < std::string > tld_match(input_size);
  std::vector < std::string > holding(2);
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    holding = identify_single_suffix(domains_ptr[i], suffix_ptr);
    domain_fragment[i] = holding[0];
    tld_match[i] = holding[1];
  }
  
  return DataFrame::create(_["domain_body"] = domain_fragment,
                           _["tld"] = tld_match,
                           _["stringsAsFactors"] = false);
}