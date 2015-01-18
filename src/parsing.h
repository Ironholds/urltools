#include <Rcpp.h>
using namespace Rcpp;

#ifndef __PARSING_INCLUDED__
#define __PARSING_INCLUDED__
class parsing{
  
  private:
    /**
     * A function for lower-casing a string; wraps tolower.
     * Used when normalising URLs prior to parsing.
     * 
     * @param url a (possibly mixed-case) URL.
     * 
     * @see parse_url, which calls this.
     * 
     * @return a string containing the lower-cased version of x.
     */
    static std::string str_tolower(std::string url);
    
  public:
  
    /**
     * A function for parsing a URL, extracting scheme,
     * domain, path, so on and so forth. Called on its own
     * by url_parse, but also vital to the operation of get_
     * and set_component.
     * 
     * @param url a URL.
     * 
     * @see get_ and set_component, which call this.
     * 
     * @return a vector consisting of the value for each component
     * part of the URL.
     */
    static std::vector < std::string > parse_url(std::string url);
    
    /**
     * A function to retrieve an individual component from a parsed
     * URL. Used in scheme(), host() et al; calls parse_url.
     * 
     * @param url a URL.
     * 
     * @param component an integer representing which value in
     * parse_url's returned vector to grab.
     * 
     * @see set_component, which allows for modification.
     * 
     * @return a string consisting of the requested URL component.
     */
    static std::string get_component(std::string url, int component);
    
    /**
     * A function to set an individual component in a parsed
     * URL. Used in "scheme<-", et al; calls parse_url.
     * 
     * @param url a URL.
     * 
     * @param component an integer representing which value in
     * parse_url's returned vector to modify.
     * 
     * @param new_value the value to insert into url[component].
     * 
     * @see get_component, which allows for retrieval.
     * 
     * @return a string consisting of the modified URL.
     */
    static std::string set_component(std::string url, int component, std::string new_value);
};
#endif