#include "http_response.hpp"
#include "headers/http_headers.hpp"
#include "string_utils.hpp"

namespace ws
{
    HttpResponse::HttpResponse() : status_code(0) {
        this->http_version = "HTTP/1.1";
    } 

    std::string HttpResponse::to_string()
    {
        std::string response;

        response.append(this->http_version);
        response.append(" ");
        response.append(ul_to_string(this->status_code));
        response.append(" ");
        response.append(this->status_msg);
        response.append("\r\n");

		HttpHeaderMap::const_iterator cit = this->headers.begin();
		HttpHeaderMap::const_iterator cite = this->headers.end();

		for (; cit != cite; cit++)
		{
			response.append(cit->first);
			response.append(": ");
			response.append(cit->second->value);
			response.append("\r\n");
		}
		
        response.append("\r\n");

		if (this->body.size() > 0)
        	response.append(this->body);

        return response;
    }

    void HttpResponse::generate_content_length_header()
    {
	    HttpHeaderContentLength *content_length_header = new HttpHeaderContentLength();
	    content_length_header->set_value(this->body.size());
	    this->headers.insert(content_length_header);
    }

    void HttpResponse::generate_content_type_header(std::string content_type)
    {
		HttpHeaderContentType *content_type_header = new HttpHeaderContentType();
		content_type_header ->set_value(content_type);
		this->headers.insert(content_type_header);
    }

}
