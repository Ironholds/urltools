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

// scan for next & separator that is not &amp;
size_t find_ampersand(std::string query, size_t pos = 0) {
  while (true) {
    size_t amp = query.find_first_of("&#", pos);
    if (amp == std::string::npos) {
      pos = amp;
      break;
    }
    if (query[amp] == '#') {
      pos = std::string::npos;
      break;
    }

    if (query.compare(amp, 5, "&amp;") == 0) {
      pos = amp + 1;
      continue;
    }
    pos = amp;
    break;
  }

  return pos;
}

std::deque < std::string > parameter::get_parameter_names_single(std::string url){
  std::deque < std::string > parsed_entry = get_query_string(url);
  std::deque < std::string > out;
  if(parsed_entry.size() < 2){
    return out;
  }
  std::string query = parsed_entry[1];
  size_t amp = 0;
  size_t eq;
  while(amp != std::string::npos) {
    eq = query.find("=", amp);
    size_t next_amp = find_ampersand(query, amp+1);
    if (eq == std::string::npos) {
      amp = next_amp;
      continue;
    }
    if (next_amp != std::string::npos && eq > next_amp) {
      amp = next_amp;
      continue;
    }
    out.push_back(query.substr(amp+1, eq-amp-1));
    amp = next_amp;
  }

  return out;
}

CharacterVector parameter::get_parameter_names(CharacterVector &urls) {
  std::set < std::string > names;
  for (int i = 0; i < urls.length(); i++) {
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if (urls[i] == R_NaString) {
      continue;
    }
    std::string str = (std::string) urls[i];
    std::deque < std::string > labels = get_parameter_names_single(str);
    for (unsigned int j = 0; j < labels.size(); j++) {
      names.insert(labels[j]);
    }
  }

  CharacterVector out(names.size());
  int ii = 0;
  for (std::set< std::string >::iterator i = names.begin();
       i != names.end();
       ii++, i++) {
    out[ii] = *i;
  }
  return out;
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
  if(holding[first_find-1] != '&' && holding[first_find-1] != '?'){
    first_find = holding.find("&" + component);
    component_size = (component.size() + 1);
    if(first_find == std::string::npos){
      return NA_STRING;
    }
  } else {
    component_size = component.size();
  }
  
  size_t next_location = find_ampersand(holding, first_find + 1);

  if(next_location == std::string::npos) {
    // check for fragment
    next_location = holding.find("#", first_find + component_size);
  }
  
  if (next_location == std::string::npos) {
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

//'@title get the values of a URL's parameters
//'@description URLs can have parameters, taking the form of \code{name=value}, chained together
//'with \code{&} symbols. \code{param_get}, when provided with a vector of URLs and a vector
//'of parameter names, will generate a data.frame consisting of the values of each parameter
//'for each URL.
//'
//'@param urls a vector of URLs
//'
//'@param parameter_names a vector of parameter names. If \code{NULL} (default), will extract
//'all parameters that are present.
//'
//'@return a data.frame containing one column for each provided parameter name. Values that
//'cannot be found within a particular URL are represented by an NA.
//'
//'@examples
//'#A very simple example
//'url <- "https://google.com:80/foo.php?this_parameter=selfreferencing&hiphop=awesome"
//'parameter_values <- param_get(url, c("this_parameter","hiphop"))
//'
//'@seealso \code{\link{url_parse}} for decomposing URLs into their constituent parts and
//'\code{\link{param_set}} for inserting or modifying key/value pairs within a query string.
//'
//'@aliases param_get url_parameter
//'@rdname param_get
//'@export
//[[Rcpp::export]]
List param_get(CharacterVector urls, Nullable<CharacterVector> parameter_names = R_NilValue){
  CharacterVector param_names;
  if (parameter_names.isNull()) {
    param_names = parameter::get_parameter_names(urls);
  } else {
    param_names = parameter_names.get();
  }
  List output;
  IntegerVector rownames = Rcpp::seq(1,urls.size());
  unsigned int column_count = param_names.size();
  
  for(unsigned int i = 0; i < column_count; ++i){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output.push_back(parameter::get_parameter(urls, Rcpp::as<std::string>(param_names[i])));
  }
  output.attr("class") = "data.frame";
  output.attr("names") = param_names;
  output.attr("row.names") = rownames;
  return output;
}

//'@title Set the value associated with a parameter in a URL's query.
//'@description URLs often have queries associated with them, particularly URLs for
//'APIs, that look like \code{?key=value&key=value&key=value}. \code{param_set}
//'allows you to modify key/value pairs within query strings, or even add new ones
//'if they don't exist within the URL.
//'
//'@param urls a vector of URLs. These should be decoded (with \code{url_decode})
//'but do not have to have been otherwise manipulated.
//'
//'@param key a string representing the key to modify the value of (or insert wholesale
//'if it doesn't exist within the URL).
//'
//'@param value a value to associate with the key. This can be a single string,
//'or a vector the same length as \code{urls}
//'
//'@return the original vector of URLs, but with modified/inserted key-value pairs. If the
//'URL is \code{NA}, the returned value will be - if the key or value are, no insertion
//'will be made.
//'
//'@examples
//'# Set a URL parameter where there's already a key for that
//'param_set("https://en.wikipedia.org/api.php?action=query", "action", "pageinfo")
//'
//'# Set a URL parameter where there isn't.
//'param_set("https://en.wikipedia.org/api.php?list=props", "action", "pageinfo")
//'
//'@seealso \code{\link{param_get}} to retrieve the values associated with multiple keys in
//'a vector of URLs, and \code{\link{param_remove}} to strip key/value pairs from a URL entirely.
//'
//'@export
//[[Rcpp::export]]
CharacterVector param_set(CharacterVector urls, String key, CharacterVector value){
  return parameter::set_parameter_vectorised(urls, key, value);
}

//'@title Remove key-value pairs from query strings
//'@description URLs often have queries associated with them, particularly URLs for
//'APIs, that look like \code{?key=value&key=value&key=value}. \code{param_remove}
//'allows you to remove key/value pairs while leaving the rest of the URL intact.
//'
//'@param urls a vector of URLs. These should be decoded with \code{url_decode} but don't
//'have to have been otherwise processed.
//'
//'@param keys a vector of parameter keys to remove.
//'
//'@return the original URLs but with the key/value pairs specified by \code{keys} removed.
//'If the original URL is \code{NA}, \code{NA} will be returned; if a specified key is \code{NA},
//'nothing will be done with it.
//'
//'@seealso \code{\link{param_set}} to modify values associated with keys, or \code{\link{param_get}}
//'to retrieve those values.
//'
//'@examples
//'# Remove multiple parameters from a URL
//'param_remove(urls = "https://en.wikipedia.org/wiki/api.php?action=list&type=query&format=json",
//'             keys = c("action","format"))
//'@export
//[[Rcpp::export]]
CharacterVector param_remove(CharacterVector urls, CharacterVector keys){
  return parameter::remove_parameter_vectorised(urls, keys);
  
}
