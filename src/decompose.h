#include <Rcpp.h>
using namespace Rcpp;

#ifndef __DECOMPOSE_INCLUDED__
#define __DECOMPOSE_INCLUDED__

class decompose{
    std::string scheme(std::string& url);
    
    std::vector < std::string > domain_and_port(std::string& url);
    
    std::string decompose::path(std::string& url);
};