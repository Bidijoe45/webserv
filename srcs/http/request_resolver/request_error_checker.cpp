#include "request_error_checker.hpp"

namespace ws
{
    RequestErrorChecker::RequestErrorChecker(HttpRequest *request) : RequestHandler(), request_(request) {}

    RequestHandlerPayload *RequestErrorChecker::handle(RequestHandlerPayload *payload)
    {
        //TODO: check request error enum
        return RequestHandler::handle(payload);
    }
}
