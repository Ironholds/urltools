#'@title Get or set a URL's scheme
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'@aliases scheme
#'@rdname scheme
#'
#'@param x a URL, or vector of URLs
#'
#'@param value a replacement value (or vector of replacement values)
#'for x's scheme.
#'
#'@seealso \code{\link{domain}}, \code{\link{port}}, \code{\link{path}},
#'\code{\link{parameters}} and \code{\link{fragment}} for other accessors.
#'
#'@examples
#'#Get a component
#'example_url <- "http://cran.r-project.org/submit.html"
#'scheme(example_url)
#'
#'#Set a component
#'scheme(example_url) <- "https"
#'
#'# NA out the URL
#'scheme(example_url) <- NA_character_
#'@import methods
#'@export
scheme <- function(x){
  return(get_component_(x,0))
}

"scheme<-" <- function(x, value) standardGeneric("scheme<-")
#'@rdname scheme
#'@export
setGeneric("scheme<-", useAsDefault = function(x, value){
  if(length(value) == 0 && is.null(value)){
    return(rm_component_(x, 0))
  }
  return(set_component_r(x, 0, value, "://"))
})

#'@title Get or set a URL's domain
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'@aliases domain
#'@rdname domain
#'
#'@param x a URL, or vector of URLs
#'
#'@param value a replacement value (or vector of replacement values)
#'for x's scheme.
#'
#'@seealso \code{\link{scheme}}, \code{\link{port}}, \code{\link{path}},
#'\code{\link{parameters}} and \code{\link{fragment}} for other accessors.
#'
#'@examples
#'#Get a component
#'example_url <- "http://cran.r-project.org/submit.html"
#'domain(example_url)
#'
#'#Set a component
#'domain(example_url) <- "en.wikipedia.org"
#'@export
domain <- function(x){
  return(get_component_(x,1))
}
"domain<-" <- function(x, value) standardGeneric("domain<-")
#'@rdname domain
#'@export
setGeneric("domain<-", useAsDefault = function(x, value){
  if(length(value) == 0 && is.null(value)){
    return(rm_component_(x, 1))
  }
  return(set_component_(x, 1, value))
})

#'@title Get or set a URL's port
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'
#'@aliases port
#'@rdname port
#'
#'@param x a URL, or vector of URLs
#'
#'@param value a replacement value (or vector of replacement values)
#'for x's port. If NULL, the port will be entirely removed.
#'
#'@seealso \code{\link{scheme}}, \code{\link{domain}}, \code{\link{path}},
#'\code{\link{parameters}} and \code{\link{fragment}} for other accessors.
#'
#'@examples
#'# Get the port
#'example_url <- "http://cran.r-project.org:80/submit.html"
#'port(example_url)
#'
#'# Set the port
#'port(example_url) <- "12"
#'
#'# Remove the port
#'port(example_url) <- NULL
#'@export
port <- function(x){
  return(get_component_(x,2))
}
"port<-" <- function(x, value) standardGeneric("port<-")
#'@rdname port
#'@export
setGeneric("port<-", useAsDefault = function(x, value){
  if(length(value) == 0 && is.null(value)){
    return(rm_component_(x, 2))
  }
  return(set_component_f(x, 2, value, ":"))
})

#'@title Get or set a URL's path
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'@aliases path
#'@rdname path
#'
#'@param x a URL, or vector of URLs
#'
#'@param value a replacement value (or vector of replacement values)
#'for x's path. If NULL, the path will be removed entirely.
#'
#'@seealso \code{\link{scheme}}, \code{\link{domain}}, \code{\link{port}},
#'\code{\link{parameters}} and \code{\link{fragment}} for other accessors.
#'
#'@examples
#'# Get the path
#'example_url <- "http://cran.r-project.org:80/submit.html"
#'path(example_url)
#'
#'# Set the path
#'path(example_url) <- "bin/windows/"
#'
#'# Remove the path
#'path(example_url) <- NULL
#'@export
path <- function(x){
  return(get_component_(x,3))
}
"path<-" <- function(x, value) standardGeneric("path<-")
#'@rdname path
#'@export
setGeneric("path<-", useAsDefault = function(x, value){
  if(length(value) == 0 && is.null(value)){
    return(rm_component_(x, 3))
  }
  return(set_component_f(x, 3, value, "/"))
})

#'@title Get or set a URL's parameters
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'
#'@aliases parameters
#'@rdname parameters
#'
#'@param x a URL, or vector of URLs
#'
#'@param value a replacement value (or vector of replacement values)
#'for x's parameters. If NULL, the parameters will be removed entirely.
#'
#'@seealso \code{\link{scheme}}, \code{\link{domain}}, \code{\link{port}},
#'\code{\link{path}} and \code{\link{fragment}} for other accessors.
#'
#'@examples
#'# Get the parameters
#'example_url <- "http://en.wikipedia.org/wiki/Aaron_Halfaker?debug=true"
#'parameters(example_url)
#'
#'# Set the parameters
#'parameters(example_url) <- "debug=false"
#'
#'# Remove the parameters
#'parameters(example_url) <- NULL
#'@export
parameters <- function(x){
  return(get_component_(x,4))
}
"parameters<-" <- function(x, value) standardGeneric("parameters<-")
#'@rdname parameters
#'@export
setGeneric("parameters<-", useAsDefault = function(x, value){
  if(length(value) == 0 && is.null(value)){
    return(rm_component_(x, 4))
  }
  return(set_component_f(x, 4, value, "?"))
})

#'@title Get or set a URL's fragment
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'@aliases fragment
#'@rdname fragment
#'
#'@param x a URL, or vector of URLs
#'
#'@param value a replacement value (or vector of replacement values)
#'for x's fragment. If NULL, the fragment will be removed entirely.
#'
#'@seealso \code{\link{scheme}}, \code{\link{domain}}, \code{\link{port}},
#'\code{\link{path}} and \code{\link{parameters}} for other accessors.
#'
#'@examples
#'#Get a component
#'example_url <- "http://en.wikipedia.org/wiki/Aaron_Halfaker?debug=true#test"
#'fragment(example_url)
#'
#'#Set a component
#'fragment(example_url) <- "production"
#'
#'#Remove a component
#'fragment(example_url) <- NULL
#'@export
#'@rdname fragment
#'@export
fragment <- function(x){
  return(get_component_(x,5))
}

"fragment<-" <- function(x, value) standardGeneric("fragment<-")
#'@rdname fragment
#'@export
setGeneric("fragment<-", useAsDefault = function(x, value){
  if(length(value) == 0 && is.null(value)){
    return(rm_component_(x, 5))
  }
  return(set_component_f(x, 5, value, "#"))
})