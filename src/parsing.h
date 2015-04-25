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
    std::string str_tolower(std::string& url);

    /**
     * A function for parsing a URL and turning it into a vector.
     * Tremendously useful for get- and set-scheme, otherwise
     * pretty much useless these days.
     * 
     * @param url a URL.
     * 
     * @see get_ and set_component, which call this.
     * 
     * @return a vector consisting of the value for each component
     * part of the URL.
     */
    std::vector < std::string > url_to_vector(std::string& url_ptr);
    std::string scheme(std::string& url);
    std::vector < std::string > domain_and_port(std::string& url);
    std::string path(std::string& url);
    std::string query(std::string& url);
  public:
    
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
    std::string get_component(std::string& url, int component);
    
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
    std::string set_component(std::string url, int component, std::string& new_value);

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
     * Decompose a URL and turn it into a list
     * 
     * @param URLs a reference to a vector of URLs
     * 
     * @return an Rcpp List that can then be classified as a data.frame.
     * 
     */
    DataFrame parse_to_df(std::vector < std::string >& urls_ptr);

};
#endif