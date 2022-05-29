#pragma once

#include <string>
#include <map>

namespace ws
{

enum HTTP_METHOD {
	GET, POST, DELETE
};

struct HttpRequest {
	HTTP_METHOD method;
	std::string url;
	std::string version;
	std::map<std::string, std::string> headers;
	//TODO: definir como se va a guardar el body de la peticion
};

} // namespace ws