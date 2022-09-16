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

    HttpMultipartBody HttpMultipartBodyParser::parse()
    {
        HttpMultipartBody body;

        std::vector<std::string> segments = string_split(this->content_, "--" + this->boundary_);

        // std::cout << "XXXXX" << std::endl;
        //
        // std::cout << "segments" << std::endl;
        // for (size_t i=0; i < segments.size(); i++)
        // {
        //     std::cout << "XX" << std::endl;
        //     std::cout << string_trim_left(segments[i], "\r\n") << std::endl;
        //     std::cout << "XX" << std::endl;
        // }

        for (size_t i=0; i < segments.size() - 1; i++)
        {
            HttpMultipartBodyPart multipart_body;
            DataBuffer buff(string_trim_left(segments[i], "\r\n"));
		    std::vector<std::string> header_block;
		    std::string segment_line = buff.get_next_line("\n");

		    while(segment_line.size() != 0)
		    {
			    header_block.push_back(segment_line);
			    segment_line = buff.get_next_line("\n");
		    }
        
		    HttpHeaderParser headers_parser(header_block);
		    HttpHeaderMap headers = headers_parser.parse_block();

            multipart_body.header_map = headers;
            multipart_body.content = string_trim_right(buff.flush(buff.size()), "\r\n");
            body.parts.push_back(multipart_body);
        }

        // std::cout << "XX-------------------XX" << std::endl;
        // for (size_t i=0; i < body.parts.size(); i++)
        // {
        //     HttpMultipartBodyPart part = body.parts[i];
        //     std::cout << "-- Part" << std::endl;
        //
        //     std::cout << "--- Headers" << std::endl;
        //     HttpHeaderMap::iterator hit = part.header_map.begin();
        //     HttpHeaderMap::iterator hite = part.header_map.end();
        //     while (hit != hite)
        //     {
        //         std::cout << hit->first << std::endl;
        //         hit++;
        //     }
        //     std::cout << "--- ------" << std::endl;
        //
        //     std::cout << "--- Content" << std::endl;
        //     std::cout << part.content << std::endl;
        //     std::cout << "--- ------" << std::endl;
        //
        //     std::cout << "-- ----" << std::endl;
        // }
        // std::cout << "XX-------------------XX" << std::endl;

        return body;
    }

    bool HttpMultipartBodyParser::is_valid()
    {
        return this->valid_;
    }
}
