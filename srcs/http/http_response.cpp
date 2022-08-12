

#include "http_response.hpp"

namespace ws
{

    std::string HttpResponse::to_string()
    {
        std::string response;
        char status_code_str[4];

        response.append(this->http_version);
        response.append(" ");
        sprintf(status_code_str, "%d", this->status_code);
        response.append(status_code_str);
        response.append(" ");
        response.append(this->status_msg);
        response.append("\r\n");

		HttpHeaderMap::const_iterator cit = this->headers.begin();
		HttpHeaderMap::const_iterator cite = this->headers.end();

		for (; cit != cite; cit++)
		{
			response.append(cit->first);
			response.append(": ");
			response.append(cit->second->get_header_value_string());
			response.append("\r\n");
		}
		
        response.append("\r\n");

		if (this->body.size() > 0)
        	response.append(this->body);

        return response;
    }

}
