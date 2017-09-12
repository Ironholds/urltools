#include "compose.h"

bool compose::emptycheck(String element){
  if(element == NA_STRING){
    return false;
  }
  return true;
}

std::string compose::compose_single(String scheme, String domain, String port, String path,
                                    String parameter, String fragment){
  
  std::string output;
  
  if(emptycheck(scheme)){
    output += scheme;
    output += "://";
  }
  
  if(emptycheck(domain)){
    output += domain;
  }
  
  if(emptycheck(port)){
      output += ":";
      output += port;
    }
  
  output += "/";
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

CharacterVector compose::compose_multiple(DataFrame parsed_urls){
  
  CharacterVector schemes = parsed_urls["scheme"];
  CharacterVector domains = parsed_urls["domain"];
  CharacterVector ports = parsed_urls["port"];
  CharacterVector paths = parsed_urls["path"];
  CharacterVector parameters = parsed_urls["parameter"];
  CharacterVector fragments = parsed_urls["fragment"];
  
  unsigned int input_size = schemes.size();
  CharacterVector output(input_size);
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output[i] = compose_single(schemes[i], domains[i], ports[i], paths[i], parameters[i],
                               fragments[i]);
  }
  
  return output;
}

//'@title Recompose Parsed URLs
//'
//'@description Sometimes you want to take a vector of URLs, parse them, perform
//'some operations and then rebuild them. \code{url_compose} takes a data.frame produced
//'by \code{\link{url_parse}} and rebuilds it into a vector of full URLs (or: URLs as full
//'as the vector initially thrown into url_parse).
//'
//'This is currently a `beta` feature; please do report bugs if you find them.
//'
//'@param parsed_urls a data.frame sourced from \code{\link{url_parse}}
//'
//'@seealso \code{\link{scheme}} and other accessors, which you may want to
//'run URLs through before composing them to modify individual values.
//'
//'@examples
//'#Parse a URL and compose it
//'url <- "http://en.wikipedia.org"
//'url_compose(url_parse(url))
//'
//'@export
//[[Rcpp::export]]
CharacterVector url_compose(DataFrame parsed_urls){
  return compose::compose_multiple(parsed_urls);
}
