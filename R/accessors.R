#'@title Get or set component parts of URLs
#'@description as in the lubridate package, individual components of a URL
#'can be both extracted or set using the relevant function call - see the
#'examples.
#'@aliases scheme
#'@rdname getset
#'
#'@examples
#'#Get a component
#'example_url <- "http://cran.r-project.org/submit.html"
#'scheme(example_url)
#'
#'#Set a component
#'scheme(example_url) <- "https"
#'@export
scheme <- function(x){
  return(v_get_component(x,0))
}
"scheme<-" <- function(x, value) standardGeneric("scheme<-")
#'@rdname getset
#'@export
setGeneric("scheme<-", useAsDefault = function(x, value){
  return(v_set_component(x, 0, value))
})

#'@rdname getset
#'@export
domain <- function(x){
  return(v_get_component(x,1))
}
"domain<-" <- function(x, value) standardGeneric("domain<-")
#'@rdname getset
#'@export
setGeneric("domain<-", useAsDefault = function(x, value){
  return(v_set_component(x, 1, value))
})

#'@rdname getset
#'@export
port <- function(x){
  return(v_get_component(x,2))
}
"port<-" <- function(x, value) standardGeneric("port<-")
#'@rdname getset
#'@export
setGeneric("port<-", useAsDefault = function(x, value){
  return(v_set_component(x, 2, value))
})

#'@rdname getset
#'@export
path <- function(x){
  return(v_get_component(x,3))
}
"path<-" <- function(x, value) standardGeneric("path<-")
#'@rdname getset
#'@export
setGeneric("path<-", useAsDefault = function(x, value){
  return(v_set_component(x, 3, value))
})

#'@rdname getset
#'@export
query <- function(x){
  return(v_get_component(x,4))
}
"query<-" <- function(x, value) standardGeneric("query<-")
#'@rdname getset
#'@export
setGeneric("query<-", useAsDefault = function(x, value){
  return(v_set_component(x, 4, value))
})

#'@rdname getset
#'@export
fragment <- function(x){
  return(v_get_component(x,5))
}
"fragment<-" <- function(x, value) standardGeneric("fragment<-")
#'@rdname getset
#'@export
setGeneric("fragment<-", useAsDefault = function(x, value){
  return(v_set_component(x, 5, value))
})