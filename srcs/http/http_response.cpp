

#include "http_response.hpp"
#include "string_utils.hpp"

namespace ws
{

    std::string HttpResponse::to_string()
    {
        std::string response;

        response.append(this->http_version);
        response.append(" ");
        response.append(int_to_string(this->status_code));
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

}
