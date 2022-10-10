#include "error_response_generator.hpp"

namespace ws
{

    ErrorResponseGenerator::ErrorResponseGenerator(const std::vector<ErrorPage> &error_pages) : error_pages_(error_pages) {}

    HttpResponse ErrorResponseGenerator::get(size_t error_code)
    {
	    this->response_.status_code = error_code;
		this->generate_body();
        return this->response_;
    }

	void ErrorResponseGenerator::generate_body()
	{
		std::string custom_error_page_path = this->get_custom_error_page_path();

		if (custom_error_page_path == "")
			this->response_.body = this->create_default_error_page();
		else
			this->response_.body = get_custom_error_page(custom_error_page_path);

        this->response_.generate_content_length_header();
        this->response_.generate_content_type_header("text/html");
	}

	std::string ErrorResponseGenerator::get_custom_error_page_path()
	{
		ServerSettings::error_pages_it it = this->error_pages_.begin();
		ServerSettings::error_pages_it ite = this->error_pages_.end();

		while (it != ite)
		{
			if (it->code == this->response_.status_code)
				return it->path;
			it++;
		}
		return "";
	}

	std::string ErrorResponseGenerator::create_default_error_page()
	{
		std::string error_page;
		std::string error = ul_to_string(this->response_.status_code) + " " + this->response_.status_msg;

		error_page.append("<!DOCTYPE html><html lang=\"en\"><head><title>");
		error_page.append(error);
		error_page.append("</title></head><body><h1>");
		error_page.append(error);
		error_page.append("</h1><p>Oops!</p><img src=\"https://http.cat/" + ul_to_string(this->response_.status_code) + "\"></body></html>");

		return error_page;
	}

	std::string ErrorResponseGenerator::get_custom_error_page(const std::string error_page_path)
	{
		FileSystem file(error_page_path);

		if (!file.is_valid())
			return this->create_default_error_page();

		return file.get_content();
	}
}
