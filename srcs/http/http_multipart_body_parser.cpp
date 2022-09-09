#include <string>

#include "http_multipart_body_parser.hpp"
#include "../utils/string_utils.hpp"

namespace ws
{
    
    HttpMultipartBodyParser::HttpMultipartBodyParser(const std::string &body, const std::string &boundary)
    {
        this->body_ = body;
    }
    
    HttpMultipartBody *HttpMultipartBodyParser::parse()
    {
        HttpMultipartBody *multipart_body = new HttpMultipartBody;


		std::vector<std::string> splitted_boundary = string_split(this->body_, "--" + this->boundary_);
		std::vector<std::string>::iterator it = splitted_boundary.begin();
		std::vector<std::string>::iterator ite = splitted_boundary.end();

		std::cout << "Boundary: " << this->boundary_ << std::endl;

		while (it != ite)
		{
            std::cout << "XXX" << std::endl;
			std::cout << *it << std::endl;
            std::cout << "XXX" << std::endl;
	 		it++;
		}
        
        return multipart_body;
    }

}
