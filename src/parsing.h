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
    std::string str_tolower(std::string url);
    
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
    std::vector < std::string > parse_url(std::string url);
    
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
    std::string get_component(std::string url, int component);
    
    /**
     * Component retrieval specifically for parameters.
     * 
     * @param urls a reference to a vector of URLs
     * 
     * @param component the name of a component to retrieve
     * the value of
     * 
     * @return a vector of the values for that component.
     */
    std::vector < std::string > get_parameter(std::vector < std::string >& urls, std::string component);
    
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
     * @param delim a delimiter, used in cases where there's no existing value.
     * @see get_component, which allows for retrieval.
     * 
     * @return a string consisting of the modified URL.
     */
    std::string set_component(std::string url, int component, std::string new_value);
};
#endif