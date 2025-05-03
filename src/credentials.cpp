#include <Rcpp.h>
using namespace Rcpp;

std::string strip_single(std::string x){
  
  std::size_t scheme_loc = x.find("://");
  if(scheme_loc == std::string::npos){
    return x;
  }
  
  std::size_t cred_loc = x.find('@');
  if(cred_loc == std::string::npos){
    return x;
  }
  if(scheme_loc > cred_loc){
    return x;
  }
  
  return x.substr(0, scheme_loc+3) + x.substr(cred_loc+1);
}

//'@title Get or remove user authentication credentials
//'@description authentication credentials appear before the domain
//'name and look like \emph{user:password}. Sometimes you want the removed,
//'or retrieved; \code{strip_credentials} and \code{get_credentials} do
//'precisely that
//'
//'@aliases creds
//'@rdname creds
//'
//'@param urls a URL, or vector of URLs
//'
//'@examples
//'# Remove credentials
//'strip_credentials("http://foo:bar@97.77.104.22:3128")
//'
//'# Get credentials
//'get_credentials("http://foo:bar@97.77.104.22:3128")
//'@export
//[[Rcpp::export]]
CharacterVector strip_credentials(CharacterVector urls){
  
  std::string holding;
  unsigned int input_size = urls.size();
  CharacterVector output(input_size);
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = strip_single(Rcpp::as<std::string>(urls[i]));
    }
  }
  
  return output;
}

void get_single(std::string x, CharacterVector& username, CharacterVector& data, unsigned int& i){
  
  std::size_t scheme_loc = x.find("://");
  if(scheme_loc == std::string::npos){
    username[i] = NA_STRING;
    data[i] = NA_STRING;
    return;
  }
  
  std::size_t cred_loc = x.find('@');
  if(cred_loc == std::string::npos){
    username[i] = NA_STRING;
    data[i] = NA_STRING;
    return;
  }
  if(scheme_loc > cred_loc){
    username[i] = NA_STRING;
    data[i] = NA_STRING;
    return;
  }
  std::string holding = x.substr(scheme_loc+3, (cred_loc - (scheme_loc+3)));
  std::size_t info = holding.find(':');
  if(info == std::string::npos){
    username[i] = holding;
    data[i] = NA_STRING;
    return;
  } else {
    username[i] = holding.substr(0, info);
    data[i] = holding.substr(info+1);
    return;
  }
}

//'@rdname creds
//'@export
//[[Rcpp::export]]
DataFrame get_credentials(CharacterVector urls){
  
  unsigned int input_size = urls.size();
  CharacterVector user(input_size);
  CharacterVector data(input_size);
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(urls[i] == NA_STRING){
      user[i] = NA_STRING;
      data[i] = NA_STRING;
    } else {
      get_single(Rcpp::as<std::string>(urls[i]), user, data, i);
    }
  }
  
  return DataFrame::create(_["username"] = user,
                           _["authentication"] = data,
                           _["stringsAsFactors"] = false);
}
