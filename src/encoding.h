#include <Rcpp.h>
using namespace Rcpp;

#ifndef __ENCODING_INCLUDED__
#define __ENCODING_INCLUDED__

/**
 * A class for applying percent-encoding to
 * arbitrary strings - optimised for URLs, obviously.
 */
class encoding{
  
  private:
  
    /**
     * A function for taking a hexadecimal element and converting
     * it to the equivalent non-hex value. Used in internal_url_decode
     * 
     * @param x a character array representing the hexed value.
     * 
     * @see to_hex for the reverse operation.
     * 
     * @return a string containing the un-hexed value of x.
     */
    static char from_hex (char x);
    
    /**
     * A function for taking a character value and converting
     * it to the equivalent hexadecimal value. Used in internal_url_encode.
     * 
     * @param x a character array representing the unhexed value.
     * 
     * @see from_hex for the reverse operation.
     * 
     * @return a string containing the now-hexed value of x.
     */
    static std::string to_hex(char x);

  public:
  
    /**
     * A function for decoding URLs. calls from_hex, and is
     * in turn called by url_decode in urltools.cpp.
     * 
     * @param url a string representing a percent-encoded URL.
     * 
     * @see internal_url_encode for the reverse operation.
     * 
     * @return a string containing the decoded URL.
     */
    static std::string internal_url_decode(std::string url);
    
    static std::string internal_url_encode(std::string url);

};

#endif