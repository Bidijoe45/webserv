#include "http_request_resolver.hpp"
#include "http_request.hpp"
#include "server_settings.hpp"
#include "http_response.hpp"

namespace ws
{

    HttpRequestResolver::HttpRequestResolver(const HttpRequest &request, const ServerSettings &settings)
    {
        this->request_ = request;
        this->settings_ = settings;
    }

    HttpResponse HttpRequestResolver::resolve()
    {

    }
    
}
