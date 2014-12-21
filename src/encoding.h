#include <Rcpp.h>
using namespace Rcpp;

#ifndef __ENCODING_INCLUDED__
#define __ENCODING_INCLUDED__

//Encoding handlers
class encoding{
  
  private:
    
    static char from_hex (char x){
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
    };
    
    static std::string to_hex(char x){
      
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

  public:

    static std::string internal_url_decode(std::string URL);
    
    static std::string urlencode(std::string & url);

};

#endif