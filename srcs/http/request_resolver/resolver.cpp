
#include "resolver.hpp"
#include "request_handler_payload.hpp"
#include "../http_response.hpp"
#include "../http_request.hpp"


namespace ws
{

    Resolver::Resolver(const ServerSettings &server_settings, const HttpRequest &request)
    {
        this->settings_ = server_settings;
        this->request_ = request;
        this->payload_ = new RequestHandlerPayload();
        this->resolve(); 
    }

    void Resolver::resolve()
    {

        RequestErrorChecker request_error_checker(&this->request_);
        SupportedMethod supported_method(&this->request_);
        LocationResolver location_resolver(&this->request_, this->settings_.locations);
        AcceptedMethod accepted_mehtod(&this->request_, &this->settings_);

        request_error_checker.set_next(&supported_method)
           ->set_next(&location_resolver)
           ->set_next(&accepted_mehtod);

        try
        {
            request_error_checker.handle(this->payload_);
        }
        catch(RequestHandler::Exception &e)
        {
            std::cout << std::string(e.what()) << ": " << e.get_error_code() << std::endl;
        }

    }

    HttpResponse *Resolver::get_response()
    {
        return this->payload_->response;
    }

}
