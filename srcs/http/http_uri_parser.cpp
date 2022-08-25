#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <sstream>

#include "http_uri_parser.hpp"
#include "http_uri.hpp"
#include "../utils/string_utils.hpp"

namespace ws
{
    HttpUriParser::HttpUriParser(const std::string &uri) : line_(uri), line_pos_(0), error_(HTTP_URI_VALID)
    {
        this->decode();
    }

    void HttpUriParser::parse_scheme()
    {
        std::string scheme = this->line_.substr(0, 7);

        if (scheme == "http://")
        {
            this->uri_.is_absolute = true;
            this->line_pos_ += 7;
        }
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
        this->line_pos_ += this->uri_.path.size();

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
        tokens = ws::string_split(str, "&");

        it = tokens.begin();
        ite = tokens.end();

        for (; it != ite; it++)
        {
            key_value = string_split(*it, "=");
            this->uri_.params.insert(std::make_pair(key_value[0], key_value[1]));
        }
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

    void HttpUriParser::decode()
    {
        std::string new_line;
        size_t i = 0;

        while (i < this->line_.size())
        {
            char character = this->line_[i];

            if (character == '%')
            {
                std::string code = this->line_.substr(i + 1, 2);
                character = parse_hex(code);
                i += 2;
            }

            new_line.push_back(character);
            i++;
        }
        this->line_ = new_line;
    }

}
