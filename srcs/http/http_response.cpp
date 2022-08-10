

#include "http_response.hpp"

namespace ws
{

    std::string HttpResponse::to_string()
    {
        std::string response;
        char status_code_str[4];

        response.append(this->version);
        response.append(" ");
        sprintf(status_code_str, "%d", this->status_code);
        response.append(status_code_str);
        response.append(" ");
        response.append(this->status_msg);
        response.append("\r\n");

        //TODO: hacer bucle para meter todos lo header
        response.append("Server: Webserv\r\n");
        response.append("Content-Length: 12\r\n");
        response.append("Content-Type: text/html\r\n");

        response.append("\r\n");
        response.append(this->body);

        return response;
    }

}
