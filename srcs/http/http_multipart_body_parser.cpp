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
		HttpHeaderParser header_line_parser;
		HttpHeader *parsed_header;
		std::string line;
		std::string header_name;
		std::string header_value;

		body = string_trim_left(body, "\r\n");

		size_t pos_end_of_header_section = body.find("\n\n");
        std::cout << "pos_end: " << pos_end_of_header_section << std::endl;
		size_t pos0 = 0;
		size_t pos1 = body.find("\r\n");

		while (pos1 != pos_end_of_header_section && pos1 != std::string::npos)
		{
			line = body.substr(pos0, pos1 - pos0);
		//	std::cout << "ASCII LINE : ";
		//	for(size_t i = 0; line[i] ; i++)
		//		std::cout << (int)line[i] << " ";
		//	std::cout << std::endl;
			header_name = this->get_header_name(line);
			if (header_name == "")
			{
				pos0 = pos1 + 1;
				pos1 = body.find("\r\n", pos0);
				continue;
			}
			header_value = this->get_header_value(line.substr(line.find(':') + 1));
			parsed_header = header_line_parser.parse(header_name, header_value);
			headers.insert(header_name, parsed_header);
			pos0 = pos1 + 1;
			pos1 = body.find("\r\n", pos0);
		}

		HttpHeaderMap::iterator it = headers.begin();
		std::cout << "RESPONSE HEADERS" << std::endl;
		for (; it != headers.end(); it++)
			std::cout << "---name: " << it->first << ", type: " << HttpHeader::header_type_to_string(it->second->type) << "---"<< std::endl;  
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
