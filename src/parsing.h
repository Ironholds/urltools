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
     * A function for extracting the scheme of a URL; part of the
     * URL parsing framework.
     * 
     * @param url a reference to a url.
     * 
     * @see url_to_vector which calls this.
     * 
     * @return a string containing the scheme of the URL if identifiable,
     * and "" if not.
     */
    std::string scheme(std::string& url);
    
    /**
     * A function for extracting the domain and port of a URL; part of the
     * URL parsing framework. Fairly unique in that it outputs a
     * vector, unlike the rest of the framework, which outputs a string,
     * since it has to handle multiple elements.
     * 
     * @param url a reference to a url. Should've been run through
     * scheme() first.
     * 
     * @see url_to_vector which calls this.
     * 
     * @return a vector containing the domain and port of the URL if identifiable,
     * and "" for each non-identifiable element.
     */
    std::vector < std::string > domain_and_port(std::string& url);
    
    /**
     * A function for extracting the path of a URL; part of the
     * URL parsing framework.
     * 
     * @param url a reference to a url. Should've been run through
     * scheme() and domain_and_port() first.
     * 
     * @see url_to_vector which calls this.
     * 
     * @return a string containing the path of the URL if identifiable,
     * and "" if not.
     */
    std::string path(std::string& url);
    
    /**
     * A function for extracting the path of a URL; part of the
     * URL parsing framework.
     * 
     * @param url a reference to a url. Should've been run through
     * scheme(), domain_and_port() and path() first.
     * 
     * @see url_to_vector which calls this.
     * 
     * @return a string containing the query string of the URL if identifiable,
     * and "" if not.
     */
    std::string query(std::string& url);
    
    /**
     * A function for parsing a URL and turning it into a vector.
     * Tremendously useful (read: everything breaks without this)
     * 
     * @param url a URL.
     * 
     * @see get_ and set_component, which call this.
     * 
     * @return a vector consisting of the value for each component
     * part of the URL.
     */
    std::vector < std::string > url_to_vector(std::string& url_ptr);
    
    /**
     * Take a URL and identify the TLD in use.
     * 
     * @param domain_ptr a reference pointer to the domain name
     * 
     * @param tlds a reference pointer to the vector of
     * TLDs
     * 
     * @see identify_multi_tld, the vectorised version.
     * 
     * @return if a match is found, {"remaining_domain_fragment","matched_tld"}.
     * If not, {"entire_domain","Not found"}
     */
    std::vector < std::string > identify_single_tld (std::string& domain_ptr, std::vector < std::string >& tld_ptr);
    
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
    std::string set_component(std::string url, int component, std::string new_value);

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
     * Decompose a vector of URLs and turn it into a data.frame.
     * 
     * @param URLs a reference to a vector of URLs
     * 
     * @return an Rcpp data.frame.
     * 
     */
    DataFrame parse_to_df(std::vector < std::string >& urls_ptr);
    
    /**
     * Take a URL and identify the TLD in use.
     * 
     * @param domains_ptr a reference pointer to a vector of
     * domain names.
     * 
     * @param tld_ptr a reference pointer to the vector of
     * TLDs
     * 
     * @see identify_multi_tld, the vectorised version.
     * 
     * @return if a match is found, {"remaining_domain_fragment","matched_tld"}.
     * If not, {"entire_domain","Not found"}
     */
    DataFrame identify_multi_tld_(std::vector < std::string >& domains_ptr, std::vector < std::string >& tld_ptr);

};
#endif