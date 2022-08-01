#pragma once

#include "http_header.hpp"

/*
REQUEST HEADERS:
	Accept               
	Accept-Charset     
	Accept-Encoding    
	Accept-Language    
	Authorization      
	Expect             
	From               
	Host               
	If-Match           
	If-Modified-Since  
	If-None-Match      
	If-Range           
	If-Unmodified-Since
	Max-Forwards       
	Proxy-Authorization
	Range              
	Referer            
	TE                 
	User-Agent
*/

namespace ws
{

  class HttpHeaderParser
  {
	public:
		HttpHeaderParser();
		HttpHeader *parse(std::string name, std::string value);
		
  };

}
