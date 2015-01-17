#include <Rcpp.h>
#include "encoding.h"
using namespace Rcpp;

char encoding::from_hex (char x){
  if(x <= '9' && x >= '0'){
    x -= '0';
  } else if(x <= 'f' && x >= 'a'){
    x -= ('a' - 10);
  } else if(x <= 'F' && x >= 'A'){
    x -= ('A' - 10);
  } else {
    x = 0;
  }
  return x;
}

std::string encoding::to_hex(char x){
  
  //Holding objects and output
  char digit_1 = (x&0xF0)>>4;
  char digit_2 = (x&0x0F);
  std::string output;
  
  //Convert
  if( 0 <= digit_1 && digit_1 <= 9){
    digit_1 += 48;
  } else if(10 <= digit_1 && digit_1 <=15){
    digit_1 += 97-10;
  }
  if(0 <= digit_2 && digit_2 <= 9){
    digit_2 += 48;
  } else if(10 <= digit_2 && digit_2 <= 15){
    digit_2 += 97-10;
  }
  
  output.append(&digit_1, 1);
  output.append(&digit_2, 1);
  return output;
}

std::string encoding::internal_url_decode(std::string url){
  
  //Create output object
  std::string result;
  
  //For each character...
  for (std::string::size_type i = 0; i <  url.size(); ++i){
    
    //If it's a +, space
    if (url[i] == '+'){
      result += ' ';
    } else if (url[i] == '%' && url.size() > i+2){//Escaped? Convert from hex and includes
      char holding_1 = encoding::from_hex(url[i+1]);
      char holding_2 = encoding::from_hex(url[i+2]);
      char holding = (holding_1 << 4) | holding_2;
      result += holding;
      i += 2;
    } else { //Permitted? Include.
      result += url[i];
    }
  }
  
  //Return
  result + "\0";
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