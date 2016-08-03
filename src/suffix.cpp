#include <Rcpp.h>
using namespace Rcpp;

std::string string_reverse(std::string x){
  std::reverse(x.begin(), x.end());
  return x;
}

//[[Rcpp::export]]
CharacterVector reverse_strings(CharacterVector strings){
  
  unsigned int input_size = strings.size();
  CharacterVector output(input_size);
  for(unsigned int i = 0; i < input_size; i++){
    if(strings[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      output[i] = string_reverse(Rcpp::as<std::string>(strings[i]));
    }
  }
  
  return output;
}

//[[Rcpp::export]]
DataFrame finalise_suffixes(CharacterVector full_domains, CharacterVector suffixes, LogicalVector wildcard){
  
  unsigned int input_size = full_domains.size();
  CharacterVector subdomains(input_size);
  CharacterVector domains(input_size);
  std::string holding;
  size_t domain_location;
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(suffixes[i] == NA_STRING || suffixes[i].size() == full_domains[i].size()){
      subdomains[i] = NA_STRING;
      domains[i] = NA_STRING;
    } else if(wildcard[i]) {
      holding = Rcpp::as<std::string>(full_domains[i]);
      holding = holding.substr(0, ((full_domains[i].size() - suffixes[i].size()) - 1));
      domain_location = holding.rfind(".");
      if(domain_location == std::string::npos){
        domains[i] = NA_STRING;
        subdomains[i] = NA_STRING;
        suffixes[i] = holding + "." + suffixes[i];
      } else {
        suffixes[i] = holding.substr(domain_location+1) + "." + suffixes[i];
        holding = holding.substr(0, domain_location);
        domain_location = holding.rfind(".");
        if(domain_location == std::string::npos){
          if(holding.size() == 0){
            domains[i] = NA_STRING;
          } else {
            domains[i] = holding;
          }
          subdomains[i] = NA_STRING;
        } else {
          domains[i] = holding.substr(domain_location+1);
          subdomains[i] = holding.substr(0, domain_location);
        }
      }
    } else {
      holding = Rcpp::as<std::string>(full_domains[i]);
      holding = holding.substr(0, ((full_domains[i].size() - suffixes[i].size()) - 1));
      domain_location = holding.rfind(".");
      if(domain_location == std::string::npos){
        subdomains[i] = NA_STRING;
        if(holding.size() == 0){
          domains[i] = NA_STRING;
        } else {
          domains[i] = holding;
        }
      } else {
        subdomains[i] = holding.substr(0, domain_location);
        domains[i] = holding.substr(domain_location+1);
      }
    }
  }
  return DataFrame::create(_["host"] = full_domains, _["subdomain"] = subdomains,
                           _["domain"] = domains, _["suffix"] = suffixes,
                           _["stringsAsFactors"] = false);
}

//[[Rcpp::export]]
CharacterVector tld_extract_(CharacterVector domains){
  
  unsigned int input_size = domains.size();
  CharacterVector output(input_size);
  std::string holding;
  size_t fragment_location;
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(domains[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      holding = Rcpp::as<std::string>(domains[i]);
      fragment_location = holding.rfind(".");
      if(fragment_location == std::string::npos || fragment_location == (holding.size() - 1)){
        output[i] = NA_STRING;
      } else {
        output[i] = holding.substr(fragment_location+1);
      }
    }
  }
  return output;
}

//[[Rcpp::export]]
CharacterVector host_extract_(CharacterVector domains){
  
  unsigned int input_size = domains.size();
  CharacterVector output(input_size);
  std::string holding;
  size_t fragment_location;
  
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(domains[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      holding = Rcpp::as<std::string>(domains[i]);
      fragment_location = holding.find(".");
      if(fragment_location == std::string::npos){
        output[i] = NA_STRING;
      } else {
        output[i] = holding.substr(0, fragment_location);
      }
    }
  }
  return output;
}