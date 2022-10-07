#include "redirect_resolver.hpp"

namespace ws
{

    RedirectResolver::RedirectResolver() : RequestHandler() {}

    RequestHandlerPayload *RedirectResolver::handle(RequestHandlerPayload *payload)
    {
        if (payload->location.redirect.code == 0)
            return RequestHandler::handle(payload);
        
		payload->response.status_code = payload->location.redirect.code;

        HttpHeaderLocation *location_header = new HttpHeaderLocation();
        location_header->uri = payload->location.redirect.to;
        location_header->value = location_header->uri.absolute_path();
		payload->response.headers.insert(location_header);

        payload->response.generate_content_length_header();

        return payload;
    }
}
