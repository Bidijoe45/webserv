#include <utility>
#include <string>

#include "http_header.hpp"
#include "http_header_map.hpp"

namespace ws
{
	HttpHeaderMap::HttpHeaderMap() {}

	HttpHeaderMap::~HttpHeaderMap()
	{
		this->deallocate_header_items();
	}

	HttpHeaderMap::HttpHeaderMap(const HttpHeaderMap &src)
	{
		HttpHeader						*header;
		HttpHeaderMap::const_iterator	it;

		for (it = src.get_headers().begin(); it != src.get_headers().end(); it++)
		{
			switch (it->second->type)
			{
				case HTTP_HEADER_ACCEPT:
					header = new HttpHeaderAccept(*static_cast<const HttpHeaderAccept*>(it->second));
					break;
				case HTTP_HEADER_HOST:
					header = new HttpHeaderHost(*static_cast<const HttpHeaderHost*>(it->second));
					break;
				default:
					header = new HttpHeaderUnknown(*static_cast<const HttpHeaderUnknown*>(it->second));
					break;
			}
			this->insert(it->first, header);
		}
	}

	HttpHeaderMap &HttpHeaderMap::operator=(const HttpHeaderMap &src)
	{
		if (this != &src)
		{
			this->deallocate_header_items();
			*this = HttpHeaderMap(src);
		}
		return *this;
	}

	void HttpHeaderMap::deallocate_header_items()
	{
		HttpHeaderMap::iterator it;

		for (it = this->_headers.begin(); it != this->_headers.end(); it++)
			delete it->second;
	}

	std::pair<HttpHeaderMap::iterator,bool> HttpHeaderMap::insert(std::string header_name, HttpHeader *header)
	{
		std::pair<std::string, HttpHeader*>	header_pair;
		header_pair = std::make_pair(header_name, header);

		return this->_headers.insert(header_pair);
	}

	HttpHeaderMap::iterator HttpHeaderMap::begin()
	{
		return this->_headers.begin();
	}

	HttpHeaderMap::const_iterator HttpHeaderMap::begin() const
	{
		return this->_headers.begin();
	}

	HttpHeaderMap::iterator HttpHeaderMap::end()
	{
		return this->_headers.end();
	}

	HttpHeaderMap::const_iterator HttpHeaderMap::end() const
	{
		return this->_headers.end();
	}

	const std::map<std::string, HttpHeader*> &HttpHeaderMap::get_headers() const
	{
		return this->_headers;
	}

	bool operator==(const HttpHeaderMap &lhs, const HttpHeaderMap &rhs)
	{
		return lhs.get_headers() == rhs.get_headers();
	}

	bool operator!=(const HttpHeaderMap &lhs, const HttpHeaderMap &rhs)
	{
		return lhs.get_headers() != rhs.get_headers();
	}
}