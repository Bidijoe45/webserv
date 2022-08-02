#include "http_request_resolver.hpp"
#include "http_request.hpp"

namespace ws
{

    HttpRequestResolver::HttpRequestResolver(const HttpRequest &request)
    {
        this->request_ = request;
    }

    void HttpRequestResolver::resolve()
    {

    }
    
}
