#include "parsing.h"

std::string parsing::string_tolower(std::string str){
  unsigned int input_size = str.size();
  for(unsigned int i = 0; i < input_size; i++){
    str[i] = tolower(str[i]);
  }
  return str;
}

std::string parsing::scheme(std::string& url){
  std::string output;
  std::size_t protocol = url.find("://");
  std::size_t definite_end = url.find(".");
  if((protocol == std::string::npos) || protocol > definite_end){
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
  std::string holding;
  unsigned int output_offset = 0;
  
  // Check for the presence of user authentication info. If it exists, dump it.
  // Use a query-check here because some people put @ info in params, baaah
  std::size_t f_param = url.find("?");
  std::size_t auth;
  if(f_param != std::string::npos){
    auth = url.substr(0, f_param).find("@");
  } else {
    auth = url.find("@");
  }
  if(auth != std::string::npos){
    url = url.substr(auth+1);
  }
  
  // ID IPv6(?)
  if(url.size() && url[0] == '['){
    std::size_t ipv6_end = url.find("]");
    if(ipv6_end != std::string::npos){
      output[0] = url.substr(1,(ipv6_end-1));
      if(ipv6_end == url.size()-1){
        url = "";
        return output;
      }
      url = url.substr(ipv6_end+1);
    }
  }
  
  // Identify the port. If there is one, push everything
  // before that straight into the output, and the remainder
  // into the holding string. If not, the entire
  // url goes into the holding string.
  std::size_t port = url.find(":");
  
  if(port != std::string::npos && url.find("/") >= port){
    output[0] += url.substr(0,port);
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
  
  // If not, there might be a query parameter or fragment
  // associated
  // with the base URL, which we need to preserve.
  std::size_t param = holding.find("?");
  
  // If there is, handle that
  if(param != std::string::npos){
    output[output_offset] = holding.substr(0, param);
    url = holding.substr(param);
    return output;
  } else {
    std::size_t frag = holding.find("#");
    if(frag != std::string::npos){
      output[output_offset] = holding.substr(0, frag);
      url = holding.substr(frag);
      return output;
    }
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

String parsing::check_parse_out(std::string x){
  
  if(x == ""){
    return NA_STRING;
  }
  return x;
}

//URL parser
CharacterVector parsing::url_to_vector(std::string url){
  
  std::string &url_ptr = url;
  
  //Output object, holding object, normalise.
  CharacterVector output(6);
  std::vector < std::string > holding(2);
  
  std::string s = scheme(url_ptr);
  
  holding = domain_and_port(url_ptr);

  //Run
  output[0] = check_parse_out(string_tolower(s));
  output[1] = check_parse_out(string_tolower(holding[0]));
  output[2] = check_parse_out(holding[1]);
  output[3] = check_parse_out(path(url_ptr));
  output[4] = check_parse_out(query(url_ptr));
  output[5] = check_parse_out(url_ptr);
  
  return output;
}

//Component retrieval
String parsing::get_component(std::string url, int component){
  return url_to_vector(url)[component];
}

//Component modification
String parsing::set_component(std::string url, int component, String new_value,
                              bool rm){
  
  if(new_value == NA_STRING && !rm){
    return NA_STRING;
  }
  std::string output;
  CharacterVector parsed_url = url_to_vector(url);
  parsed_url[component] = new_value;
  
  if(parsed_url[0] != NA_STRING){
    output += parsed_url[0];
    output += "://";
  }
  
  if(parsed_url[1] != NA_STRING){
    output += parsed_url[1];
  }
  
  if(parsed_url[2] != NA_STRING){
    output += ":";
    output += parsed_url[2];
  }
  
  if(parsed_url[3] != NA_STRING){
    output += "/";
    output += parsed_url[3];
  }
  
  if(parsed_url[4] != NA_STRING){
    output += "?";
    output += parsed_url[4];
  }
  
  if(parsed_url[5] != NA_STRING){
    output += "#";
    output += parsed_url[5];
  }
  
  return output;
}

DataFrame parsing::parse_to_df(CharacterVector& urls_ptr){
  
  //Input and holding objects
  unsigned int input_size = urls_ptr.size();
  CharacterVector holding(6);
  
  //Output objects
  CharacterVector schemes(input_size);
  CharacterVector domains(input_size);
  CharacterVector ports(input_size);
  CharacterVector paths(input_size);
  CharacterVector parameters(input_size);
  CharacterVector fragments(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    
    // Handle NAs on input
    if(urls_ptr[i] == NA_STRING){
      
      schemes[i] = NA_STRING;
      domains[i] = NA_STRING;
      ports[i] = NA_STRING;
      paths[i] = NA_STRING;
      parameters[i] = NA_STRING;
      fragments[i] = NA_STRING;
      
    } else {
      holding = url_to_vector(Rcpp::as<std::string>(urls_ptr[i]));
      schemes[i] = holding[0];
      domains[i] = holding[1];
      ports[i] = holding[2];
      paths[i] = holding[3];
      parameters[i] = holding[4];
      fragments[i] = holding[5];
    }
  }
  
  return DataFrame::create(_["scheme"] = schemes,
                           _["domain"] = domains,
                           _["port"] = ports,
                           _["path"] = paths,
                           _["parameter"] = parameters,
                           _["fragment"] = fragments,
                           _["stringsAsFactors"] = false);
}

//'@title split URLs into their component parts
//'@description \code{url_parse} takes a vector of URLs and splits each one into its component
//'parts, as recognised by RfC 3986.
//'
//'@param urls a vector of URLs
//'
//'@details It's useful to be able to take a URL and split it out into its component parts - 
//'for the purpose of hostname extraction, for example, or analysing API calls. This functionality
//'is not provided in base R, although it is provided in \code{\link[httr]{parse_url}}; that
//'implementation is entirely in R, uses regular expressions, and is not vectorised. It's
//'perfectly suitable for the intended purpose (decomposition in the context of automated
//'HTTP requests from R), but not for large-scale analysis.
//'
//'Note that user authentication/identification information is not extracted;
//'this can be found with \code{\link{get_credentials}}.
//'
//'@return a data.frame consisting of the columns scheme, domain, port, path, query
//'and fragment. See the '\href{https://datatracker.ietf.org/doc/html/rfc3986}{relevant IETF RfC} for
//'definitions. If an element cannot be identified, it is represented by an empty string.
//'
//'@examples
//'url_parse("https://en.wikipedia.org/wiki/Article")
//'
//'@seealso \code{\link{param_get}} for extracting values associated with particular keys in a URL's
//'query string, and \code{\link{url_compose}}, which is \code{url_parse} in reverse.
//'
//'@export
//[[Rcpp::export]]
DataFrame url_parse(CharacterVector urls){
  CharacterVector& urls_ptr = urls;
  return parsing::parse_to_df(urls_ptr);
}

//[[Rcpp::export]]
CharacterVector get_component_(CharacterVector urls, int component){
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] != NA_STRING){
      output[i] = parsing::get_component(Rcpp::as<std::string>(urls[i]), component);
    } else {
      output[i] = NA_STRING;
    }
  }
  return output;
}

//[[Rcpp::export]]
CharacterVector set_component_(CharacterVector urls, int component,
                               CharacterVector new_value){
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  
  if(new_value.size() == 1){
    for (unsigned int i = 0; i < input_size; ++i){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      
      output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, new_value[0], false);
    }
  } else if(new_value.size() == input_size){
    for (unsigned int i = 0; i < input_size; ++i){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      
      output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, new_value[i], false);
    }
  } else {
    Rcpp::stop("The number of new values must either be 1, or match the number of URLs");
  }

  return output;
}

//[[Rcpp::export]]
CharacterVector set_component_r(CharacterVector urls, int component,
                                CharacterVector new_value,
                                std::string comparator){
  
  // Output object
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  
  // Comparator checking objects
  std::string holding;
  String to_use;
  unsigned int holding_size;
  unsigned int comparator_length = comparator.size();
  
  // Otherwise, if we've got a single value, iterate
  if(new_value.size() == 1){
    if(new_value[0] == NA_STRING){
      to_use = new_value[0];
    } else {
      holding = new_value[0];
      holding_size = holding.size();
      if(holding_size < comparator_length){
        to_use = holding;
      } else {
        if(holding.substr((holding_size - comparator_length), comparator_length) == comparator){
          to_use = holding.substr(0, (holding_size - comparator_length));
        } else {
          to_use = holding;
        }
      }
    }
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
        
      output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, to_use, false);
    }
    // If we've got multiple values, it's just a rejigging of the same
  } else if(new_value.size() == input_size){
    
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      
      if(new_value[i] == NA_STRING){
        to_use = new_value[i];
      } else {
        holding = new_value[i];
        holding_size = holding.size();
        if(holding_size < comparator_length){
          to_use = holding;
        } else {
          if(holding.substr((holding_size - comparator_length), comparator_length) == comparator){
            to_use = holding.substr(0, (holding_size - comparator_length));
          } else {
            to_use = holding;
          }
        }
      }
      output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, to_use, false);
    }
  } else {
    Rcpp::stop("The number of new values must either be 1, or match the number of URLs");
  }
  
  return output;
  
}

//[[Rcpp::export]]
CharacterVector set_component_f(CharacterVector urls, int component,
                                CharacterVector new_value,
                                std::string comparator){
  
  // Output object
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  
  // Comparator checking objects
  std::string holding;
  String to_use;
  unsigned int holding_size;
  unsigned int comparator_length = comparator.size();
  
  // Otherwise, if we've got a single value, iterate
  if(new_value.size() == 1){
    if(new_value[0] == NA_STRING){
      to_use = new_value[0];
    } else {
      holding = new_value[0];
      holding_size = holding.size();
      if(holding_size < comparator_length){
        to_use = holding;
      } else {
        if(holding.substr(0, comparator_length) == comparator){
          to_use = holding.substr(comparator_length, (holding_size - comparator_length));
        } else {
          to_use = holding;
        }
      }
    }
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      
      output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, to_use, false);
    }
    // If we've got multiple values, it's just a rejigging of the same
  } else if(new_value.size() == input_size){
    
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      
      if(new_value[i] == NA_STRING){
        to_use = new_value[i];
      } else {
        holding = new_value[i];
        holding_size = holding.size();
        if(holding_size < comparator_length){
          to_use = holding;
        } else {
          if(holding.substr(0, comparator_length) == comparator){
            to_use = holding.substr(comparator_length, (holding_size - comparator_length));
          } else {
            to_use = holding;
          }
        }
      }
      output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, to_use, false);
    }
  } else {
    Rcpp::stop("The number of new values must either be 1, or match the number of URLs");
  }
  
  return output;
}

//[[Rcpp::export]]
CharacterVector rm_component_(CharacterVector urls, int component){
  
  if(component < 2){
    Rcpp::stop("Scheme and domain are required components");
  }
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  for (unsigned int i = 0; i < input_size; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    
    output[i] = parsing::set_component(Rcpp::as<std::string>(urls[i]), component, NA_STRING, true);
  }
  return output;
}