#include "request_error_checker.hpp"
#include "request_handler.hpp"
#include "http_request.hpp"

namespace ws
{
    RequestErrorChecker::RequestErrorChecker(const HttpRequest &request) : RequestHandler(), request_(request) {}

    RequestHandlerPayload *RequestErrorChecker::handle(RequestHandlerPayload *payload)
    {
		switch (this->request_.error)
		{
			case HttpRequest::NO_ERROR:
				break;
			case HttpRequest::BAD_REQUEST:
				throw RequestHandler::Exception(400);
			case HttpRequest::INVALID_VERSION:
				throw RequestHandler::Exception(505);
			case HttpRequest::LENGTH_REQUIRED:
				throw RequestHandler::Exception(411);
			case HttpRequest::NOT_IMPLEMENTED:
				throw RequestHandler::Exception(501);
			case HttpRequest::BODY_TOO_LARGE:
			    throw RequestHandler::Exception(413);
			default:
				throw RequestHandler::Exception(400);
		}
        return RequestHandler::handle(payload);
    }
}
