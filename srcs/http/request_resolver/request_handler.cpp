#include "request_handler.hpp"

namespace ws
{
    RequestHandler::RequestHandler() : next_handler_(NULL) {}

    RequestHandler *RequestHandler::set_next(RequestHandler *handler)
    {
        this->next_handler_ = handler;
        return handler;
    }

    RequestHandlerPayload *RequestHandler::handle(RequestHandlerPayload *payload)
    {
        if (!this->next_handler_)
            return payload;

        return this->next_handler_->handle(payload);
    }

    RequestHandler::Exception::Exception(size_t error_code) : error_code_(error_code) {}

    const char *RequestHandler::Exception::what()
    {
        return "Request Handler Exception";
    }

    size_t RequestHandler::Exception::get_error_code()
    {
        return this->error_code_;
    }
}
