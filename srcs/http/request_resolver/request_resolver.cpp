#include <iostream>

#include "request_resolver.hpp"
#include "request_handler_payload.hpp"
#include "../http_response.hpp"
#include "../http_request.hpp"
#include "error_response_generator.hpp"
#include "../../utils/http_message_map.hpp"
#include "cgi_caller.hpp"
#include "accepted_method.hpp"
#include "error_response_generator.hpp"
#include "file_path_resolver.hpp"
#include "location_resolver.hpp"
#include  "method_caller.hpp"
#include "redirect_resolver.hpp"
#include "request_error_checker.hpp"
#include "request_handler.hpp"
#include "supported_methods.hpp"

namespace ws
{

    RequestResolver::RequestResolver(const ServerSettings &server_settings,
                        const HttpRequest &request,
                        const EnvMap &env_map,
                        const Connection &connection,
                        const ContentTypeMap &content_type_map,
                        const HttpMessageMap &http_message_map)
                        : http_message_map_(http_message_map)
    {
        this->settings_ = server_settings;
        this->request_ = request;
        this->env_map_ = env_map;
        this->connection_ = connection; 
        this->payload_ = new RequestHandlerPayload();
        this->content_type_map_ = content_type_map;
        this->resolve(); 
    }

    RequestResolver::~RequestResolver()
    {
        delete this->payload_;
    }


    void RequestResolver::resolve()
    {
        RequestErrorChecker request_error_checker(this->request_);
        SupportedMethods supported_method(&this->request_);
        LocationResolver location_resolver(&this->request_, this->settings_.locations);
        AcceptedMethod accepted_method(&this->request_, &this->settings_);
        RedirectResolver redirect_resolver;
        FilePathResolver file_path_resolver(&this->request_);
        CGICaller cgi_caller(&this->request_, &this->env_map_, &this->connection_);
        MethodCaller method_caller(&this->request_, this->content_type_map_);

        request_error_checker.set_next(&supported_method)
           ->set_next(&location_resolver)
           ->set_next(&accepted_method)
           ->set_next(&redirect_resolver)
           ->set_next(&file_path_resolver)
           ->set_next(&cgi_caller)
           ->set_next(&method_caller);

        try
        {
            request_error_checker.handle(this->payload_);
        }
        catch(RequestHandler::Exception &e)
        {
            ErrorResponseGenerator error_generator(this->settings_.error_pages, this->http_message_map_);
            this->payload_->response = error_generator.get(e.get_error_code());
            std::cout << std::string(e.what()) << ": " << e.get_error_code() << std::endl;
        }
        
        this->payload_->response.status_msg = this->http_message_map_.get_msg(this->payload_->response.status_code);
    }

    HttpResponse RequestResolver::get_response()
    {
        return this->payload_->response;
    }

}
