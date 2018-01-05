#include <Rcpp.h>
using namespace Rcpp;

#ifndef __ENCODING_INCLUDED__
#define __ENCODING_INCLUDED__

/**
 * A namespace for applying percent-encoding to
 * arbitrary strings - optimised for URLs, obviously.
 */
namespace encoding{

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
    char from_hex (char x);
    
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
    std::string to_hex(char x);

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
    std::string internal_url_decode(std::string url);
    
    /**
     * A function for encoding URLs. calls to_hex, and is
     * in turn called by url_encode in urltools.cpp.
     * 
     * @param url a string representing a URL.
     * 
     * @see internal_url_decode for the reverse operation.
     * 
     * @return a string containing the percent-encoded version of "url".
     */
    std::string internal_url_encode(std::string url);

}

#endif
