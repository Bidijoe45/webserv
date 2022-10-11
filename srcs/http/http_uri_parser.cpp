#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <sstream>

#include "http_uri.hpp"
#include "http_uri_parser.hpp"
#include "../utils/string_utils.hpp"

namespace ws
{
    HttpUriParser::HttpUriParser(const std::string &uri) : line_(uri), line_pos_(0), error_(HTTP_URI_VALID)
    {
    }

    void HttpUriParser::parse_scheme()
    {
        size_t scheme_end = this->line_.find_first_of(":");
        std::string scheme = this->line_.substr(0, scheme_end);

        if (scheme_end == std::string::npos)
            return;

        if (scheme == "http")
            this->uri_.is_absolute = true;

        this->uri_.schema = scheme;
        this->line_pos_ += scheme_end + 2;
    }

    void HttpUriParser::parse_host()
    {
        size_t colon_pos = this->line_.find_first_of(':', this->line_pos_);
        size_t slash_pos = this->line_.find_first_of('/', this->line_pos_);

        if (colon_pos != std::string::npos)
            this->uri_.host = this->line_.substr(this->line_pos_, colon_pos - this->line_pos_);
        else if (slash_pos != std::string::npos)
            this->uri_.host = this->line_.substr(this->line_pos_, slash_pos - this->line_pos_);

        this->line_pos_ += this->uri_.host.size();

        if (colon_pos != std::string::npos)
            this->parse_port();
    }

    void HttpUriParser::parse_port()
    {
        size_t slash_pos = this->line_.find_first_of('/', this->line_pos_);
        std::string port_str;

        this->line_pos_ += 1;
        if (slash_pos == std::string::npos)
            port_str = this->line_.substr(this->line_pos_);
        else
            port_str = this->line_.substr(this->line_pos_, slash_pos - this->line_pos_);

        this->uri_.port = atol(port_str.c_str());
        this->line_pos_ += port_str.size();
    }

    void HttpUriParser::parse_path()
    {
        size_t question_mark_pos = this->line_.find_first_of('?', this->line_pos_);

        if (question_mark_pos == std::string::npos)
            this->uri_.path = this->line_.substr(this->line_pos_);
        else
            this->uri_.path = this->line_.substr(this->line_pos_, question_mark_pos - this->line_pos_);
        this->uri_.path = compress_slash(this->uri_.path);
        this->uri_.path = decode(this->uri_.path);
        this->line_pos_ = question_mark_pos;

        if (question_mark_pos != std::string::npos)
            this->parse_query();

    }

    void HttpUriParser::parse_query()
    {
        std::vector<std::string> tokens;
        std::vector<std::string>::iterator it; 
        std::vector<std::string>::iterator ite;
        std::vector<std::string> key_value;
        std::string str;

        this->line_pos_ += 1;
        str = this->line_.substr(this->line_pos_);
        this->uri_.query = str;
    }

    HttpUri HttpUriParser::parse()
    {
        try
        {
            this->parse_scheme();
            if (this->uri_.is_absolute)
                this->parse_host();
            this->parse_path();
        }
        catch (std::runtime_error &e)
        {
            std::cout << "Exception: HttpUriParser" << std::endl;
        }

        return this->uri_;
    }

    HTTP_URI_ERROR HttpUriParser::get_error()
    {
        return this->error_;
    }

    bool HttpUriParser::uri_is_valid()
    {
        return (this->error_ == HTTP_URI_VALID);
    }

    int HttpUriParser::parse_hex(const std::string &str)
    {
        int ret;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> ret;
        return ret;
    }

    std::string HttpUriParser::decode(const std::string &str)
    {
        std::string new_line;
        size_t i = 0;

        while (i < str.size())
        {
            char character = str[i];

            if (character == '%')
            {
                std::string code = str.substr(i + 1, 2);
                character = parse_hex(code);
                i += 2;
            }

            new_line.push_back(character);
            i++;
        }

        return new_line;
    }

}
