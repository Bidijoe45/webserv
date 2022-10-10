#include "accepted_method.hpp"

namespace ws
{
    AcceptedMethod::AcceptedMethod(HttpRequest *request, ServerSettings *settings)
        : RequestHandler(), request_(request) {}

    RequestHandlerPayload *AcceptedMethod::handle(RequestHandlerPayload *payload)
    {
        std::vector<HTTP_METHOD>::iterator accepted_method;

        accepted_method = std::find(payload->location.methods.begin(), payload->location.methods.end(), this->request_->request_line.method);

        if (accepted_method == payload->location.methods.end())
            throw RequestHandler::Exception(405);

        return RequestHandler::handle(payload);
    }
}
