#include <iostream>
#include <string>

#include "http_multipart_body_parser.hpp"
#include "../utils/string_utils.hpp"
#include "http_header_parser.hpp"

namespace ws
{
    HttpMultipartBodyParser::HttpMultipartBodyParser(const std::string &body_content, const std::string &boundary)
        : valid_(true), content_(body_content), boundary_(boundary)
    {}

	std::string HttpMultipartBodyParser::get_header_name(std::string line)
	{
		std::string header_name;
		size_t colon_pos = line.find(':');

		if (colon_pos == std::string::npos)
			return "";

		header_name = line.substr(0, colon_pos); 
		header_name = string_to_lower(header_name, header_name.size());

		if (!is_token(header_name))
			return "";

		return header_name;
	}

	std::string HttpMultipartBodyParser::get_header_value(std::string line)
	{
		std::string header_value;

		size_t value_start = line.find_first_not_of(" \t\n");
		if (value_start == std::string::npos)
			return "";
		size_t value_end = line.find_last_not_of(" \t");

		header_value = line.substr(value_start, value_end - value_start + 1);
		
		return header_value;
	}

	HttpHeaderMap HttpMultipartBodyParser::parse_headers(std::string body)
	{
		HttpHeaderMap headers;

		return headers;
	}

    HttpMultipartBody HttpMultipartBodyParser::parse()
    {
        HttpMultipartBody body;

        std::vector<std::string> segments = string_split(this->content_, "--" + this->boundary_);

        std::cout << "XXXXX" << std::endl;

        std::cout << "segments" << std::endl;
        for (size_t i=0; i < segments.size(); i++)
        {
            std::cout << "XX" << std::endl;
            std::cout << string_trim_left(segments[i], "\r\n") << std::endl;
            std::cout << "XX" << std::endl;
        }

        for (size_t i=0; i < segments.size() - 1; i++)
        {
            this->parse_headers(string_trim_left(segments[i], "\n"));
        }

        std::cout << "XX-------------------XX" << std::endl;
        for (size_t i=0; i < body.parts.size(); i++)
        {
            HttpMultipartBodyPart part = body.parts[i];
            std::cout << "-- Part" << std::endl;

            std::cout << "--- Headers" << std::endl;
            HttpHeaderMap::iterator hit = part.header_map.begin();
            HttpHeaderMap::iterator hite = part.header_map.end();
            while (hit != hite)
                std::cout << hit->first << std::endl;
            std::cout << "--- ------" << std::endl;

            std::cout << "--- Content" << std::endl;
            std::cout << part.content << std::endl;
            std::cout << "--- ------" << std::endl;

            std::cout << "-- ----" << std::endl;
        }
        std::cout << "XX-------------------XX" << std::endl;

        return body;
    }

    bool HttpMultipartBodyParser::is_valid()
    {
        return this->valid_;
    }
}
