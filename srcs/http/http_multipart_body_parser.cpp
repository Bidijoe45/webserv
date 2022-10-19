#include <iostream>
#include <string>

#include "http_header.hpp"
#include "http_multipart_body_parser.hpp"
#include "../utils/string_utils.hpp"
#include "http_header_parser.hpp"
#include "../server/data_buffer.hpp"

namespace ws
{
    HttpMultipartBodyParser::HttpMultipartBodyParser(const std::string &body_content, const std::string &boundary)
        : valid_(true), content_(body_content), boundary_(boundary)
    {}

    HttpMultipartBody HttpMultipartBodyParser::parse()
    {
        HttpMultipartBody body;

        if (this->content_.size() < MIN_N_CHARS_MULTIPART_BODY)
        {
            this->valid_ = false;
            return body;
        }

        std::vector<std::string> segments = string_split(this->content_, "--" + this->boundary_);

        if (segments.size() <= 1)
        {
            this->valid_ = false;
            return body;
        }
        
        segments.back() = string_trim_right(segments.back(), "\r\n");

        if (segments.back() != "--")
        {
            this->valid_ = false;
            return body;
        }

        segments.erase(segments.end() - 1);

        for (size_t i=0; i < segments.size(); i++)
        {
            HttpMultipartBodyPart multipart_body;
            DataBuffer buff(string_trim_left(segments[i], "\r\n"));
		    std::vector<std::string> header_block;
		    std::string segment_line = buff.get_next_line();

		    while(segment_line.size() != 0)
		    {
			    header_block.push_back(segment_line);
			    segment_line = buff.get_next_line();
		    }

		    HttpHeaderParser headers_parser(header_block);
		    HttpHeaderMap headers = headers_parser.parse_block();

		    if (!headers_parser.is_valid())
		    {
                this->valid_ = false;
                return body;
		    }

            HttpHeaderMap::iterator cd_header = headers.find(HTTP_HEADER_CONTENT_DISPOSITION);
            if (cd_header == headers.end())
            {
                this->valid_ = false;
                return body;
            }

            multipart_body.header_map = headers;
            multipart_body.content = string_trim_right(buff.flush(buff.size()), "\r\n");
            body.parts.push_back(multipart_body);
        }

        return body;
    }

    bool HttpMultipartBodyParser::is_valid()
    {
        return this->valid_;
    }
}
