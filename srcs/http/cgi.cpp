#include <stdlib.h>

#include "cgi.hpp"

namespace ws
{

    CGI::CGI(const HttpRequest &request)
    {
		setenv("REQUEST_METHOD", request.method_to_string().c_str(), 1);
    }

    void CGI::execute()
    {

    }
    
}
