#include <iostream>

#include "../http_response.hpp"
#include "../http_request.hpp"
#include "../../settings/server_settings.hpp"
#include "../../utils/string_utils.hpp"
#include "request_handler_payload.hpp"

namespace ws
{
    class RequestHandler
    {
        public:
            RequestHandler() : next_handler_(NULL) {}

            RequestHandler *set_next(RequestHandler *handler)
            {
                this->next_handler_ = handler;
                return handler;
            }

            virtual RequestHandlerPayload *handle(RequestHandlerPayload *response)
            {
                if (!this->next_handler_)
                    return response;

                return this->next_handler_->handle(response);
            }

            class Exception : public std::exception {
                public:
                    Exception(size_t error_code) : error_code_(error_code) {}

                    const char * what ()
                    {
                        return "Request Handler Exception";
                    }

                    size_t get_error_code()
                    {
                        return this->error_code_;
                    }

                private:
                    size_t error_code_;

            };

        private:
            RequestHandler *next_handler_;
    };

    class RequestErrorChecker : public RequestHandler
    {
        public:
            RequestErrorChecker(HttpRequest *request) : RequestHandler(), request_(request) {}

            RequestHandlerPayload *handle(RequestHandlerPayload *payload)
            {
                //TODO: check request error enum
                return RequestHandler::handle(payload);
            }

        private:
            HttpRequest *request_;
    };

    class SupportedMethod: public RequestHandler
    {
        public:
            SupportedMethod(HttpRequest *request)
                : RequestHandler(), request_(request)
            {
                this->supported_methods_.push_back(HTTP_METHOD_GET);
                this->supported_methods_.push_back(HTTP_METHOD_POST);
                this->supported_methods_.push_back(HTTP_METHOD_DELETE);
            }

            RequestHandlerPayload *handle(RequestHandlerPayload *payload)
            {
                std::vector<HTTP_METHOD>::iterator supported_method;

                supported_method = std::find(this->supported_methods_.begin(), this->supported_methods_.end(), this->request_->request_line.method);
                if (supported_method == this->supported_methods_.end())
                    throw RequestHandler::Exception(501);

                return RequestHandler::handle(payload);
            }

        private:
            HttpRequest *request_;
            std::vector<HTTP_METHOD> supported_methods_;
    };

    class LocationResolver: public RequestHandler
    {
        public:
            LocationResolver(HttpRequest *request, const std::vector<Location> &locations)
                : RequestHandler(), request_(request)
            {
                std::vector<Location>::const_iterator it = locations.begin();
                std::vector<Location>::const_iterator ite = locations.end();

                for (; it != ite; it++)
                {
                    std::vector<std::string> split = string_split((*it).path, "/");
                    this->splitted_locations_.push_back(split);
                }
                this->locations_ = locations;
            }

            RequestHandlerPayload *handle(RequestHandlerPayload *payload)
            {
                payload->location = resolve(this->request_->request_line.uri);

                if (payload->location == NULL)
                    throw RequestHandler::Exception(404);

                std::cout << "Solved Location: " <<  payload->location->path << std::endl;

                return RequestHandler::handle(payload);
            }

        private:
            Location *resolve(const HttpUri &uri)
            {
                std::vector<std::string> split_uri_path = string_split(uri.path, "/");
                Location *location = NULL;
                size_t location_i = 0;
                size_t splitted_location_i = 0;
                size_t hightes_score = 0;

                for (; location_i < this->locations_.size(); location_i++)
                {
                    size_t score = this->compare_locations(split_uri_path, this->splitted_locations_[location_i]);
                    if (score > hightes_score)
                        location = new Location(this->locations_[location_i]);
                }
                
                return location;
            }
            
            size_t compare_locations(const std::vector<std::string> &vec1, const std::vector<std::string> &vec2)
            {
                size_t size = (vec1.size() < vec2.size()) ? vec1.size() : vec2.size();
                size_t equal_segments = 0;

                 if (vec2.size() == 0)
                     equal_segments += 1;
                
                 for (size_t i=0; i < size; i++)
                 {
                     if (vec1[i] == vec2[i])
                         equal_segments += 2;
                     else
                         return equal_segments;
                 }

                return equal_segments;
            }

            std::vector<Location> locations_;
            std::vector<std::vector<std::string> > splitted_locations_;
            HttpRequest *request_;
    };

    class AcceptedMethod : public RequestHandler
    {
        public:
            AcceptedMethod(HttpRequest *request, ServerSettings *settings)
                : RequestHandler(), request_(request) {}

            RequestHandlerPayload *handle(RequestHandlerPayload *payload)
            {
                std::vector<HTTP_METHOD>::iterator accepted_method;

                accepted_method = std::find(payload->location->methods.begin(), payload->location->methods.end(), this->request_->request_line.method);

                if (accepted_method == payload->location->methods.end())
                    throw RequestHandler::Exception(405);

                return RequestHandler::handle(payload);
            }

        private:
            HttpRequest *request_;
            ServerSettings *settings_;

    };

}
