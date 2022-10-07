#include "supported_methods.hpp"

namespace ws
{
    SupportedMethods::SupportedMethods(HttpRequest *request) : RequestHandler(), request_(request)
    {
        this->supported_methods_.push_back(HTTP_METHOD_GET);
        this->supported_methods_.push_back(HTTP_METHOD_POST);
        this->supported_methods_.push_back(HTTP_METHOD_DELETE);
    }

    RequestHandlerPayload *SupportedMethods::handle(RequestHandlerPayload *payload)
    {
        std::vector<HTTP_METHOD>::iterator supported_method;

        supported_method = std::find(this->supported_methods_.begin(), this->supported_methods_.end(), this->request_->request_line.method);
        if (supported_method == this->supported_methods_.end())
            throw RequestHandler::Exception(501);

        return RequestHandler::handle(payload);
    }
}
