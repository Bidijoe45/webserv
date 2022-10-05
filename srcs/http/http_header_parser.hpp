#pragma once

#include <string>
#include <vector>

#include "http_header.hpp"
#include "http_header_map.hpp"

namespace ws
{

  class HttpHeaderParser
  {
	public:
		HttpHeaderParser(const std::vector<std::string> &block);
		HttpHeaderMap parse_block();
		bool is_valid();

	private:
		HttpHeader *parse_header(const std::string &name, const std::string &value);
		void combine_value(HttpHeader *header, const std::string &added_value);
		std::string get_header_name();
		std::string	get_header_value();
		void advance(size_t n);
		void skipOWS();

		std::vector<std::string> block_;
		std::string current_line_;
		size_t line_pos_;
		bool valid_;
  };

}
