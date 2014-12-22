#include <Rcpp.h>
#include "encoding.h"
using namespace Rcpp;

std::string encoding::internal_url_decode(std::string URL){
  
  //Create output object
  std::string result;
  
  //For each character...
  for (std::string::size_type i = 0; i <  URL.size(); ++i){
    
    //If it's a +, space
    if (URL[i] == '+'){
      result += ' ';
    } else if (URL[i] == '%' && URL.size() > i+2){//Escaped? Convert from hex and includes
      char holding_1 = encoding::from_hex(URL[i+1]);
      char holding_2 = encoding::from_hex(URL[i+2]);
      char holding = (holding_1 << 4) | holding_2;
      result += holding;
      i += 2;
    } else { //Permitted? Include.
      result += URL[i];
    }
  }
  
  //Return
    return result;
}

std::string encoding::internal_url_encode(std::string url){

  //Note the unreserved characters, create an output string
  std::string unreserved_characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._~-";
  std::string output = "";
  
  //For each character..
  for(int i=0; i < (signed) url.length(); i++){
    
    //If it's in the list of reserved ones, just pass it through
    if (unreserved_characters.find_first_of(url[i]) != std::string::npos){
      output.append(&url[i], 1);
    //Otherwise, append in an encoded form.
    } else {
      output.append("%");
      output.append(to_hex(url[i]));
    }
  }
  
  //Return
  return output;
}