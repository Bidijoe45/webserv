#pragma once

#include "request_handler_payload.hpp"

namespace ws
{
    class RequestHandler
    {
        public:
            RequestHandler();
            RequestHandler *set_next(RequestHandler *handler);
            virtual RequestHandlerPayload *handle(RequestHandlerPayload *payload);

            class Exception : public std::exception {
                public:
                    Exception(size_t error_code);
                    const char *what();
                    size_t get_error_code();

                private:
                    size_t error_code_;
            };

        private:
            RequestHandler *next_handler_;
    };
}
