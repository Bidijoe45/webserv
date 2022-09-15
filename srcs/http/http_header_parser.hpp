#pragma once

#include <string>

#include "http_header.hpp"
#include "http_header_map.hpp"
#include "../server/data_buffer.hpp"

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
		HttpHeaderParser(const DataBuffer &buff);
		HttpHeaderMap parse_block();
		bool is_valid();

	private:
		HttpHeader *parse_header(const std::string &name, const std::string &value);
		void combine_value(HttpHeader *header, const std::string &added_value);
		std::string get_header_name();
		std::string	get_header_value();
		void advance(size_t n);
		void skipOWS();

		DataBuffer buff_;
		std::string current_line_;
		size_t line_pos_;
		bool valid_;
  };

}
