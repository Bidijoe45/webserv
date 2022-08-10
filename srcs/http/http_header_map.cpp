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
			header = alloc_new_header(it->second);
			this->insert(it->first, header);
		}
	}

	HttpHeaderMap &HttpHeaderMap::operator=(const HttpHeaderMap &src)
	{
		if (this == &src)
			return *this;

		this->deallocate_header_items();

		HttpHeader						*header;
		HttpHeaderMap::const_iterator	it;

		for (it = src.get_headers().begin(); it != src.get_headers().end(); it++)
		{
			header = alloc_new_header(it->second);
			this->insert(it->first, header);
		}

		return *this;
	}

	HttpHeader *HttpHeaderMap::alloc_new_header(HttpHeader *header)
	{
		switch (header->type)
		{
			case HTTP_HEADER_ACCEPT:
				return new HttpHeaderAccept(*static_cast<const HttpHeaderAccept*>(header));
			case HTTP_HEADER_HOST:
				return new HttpHeaderHost(*static_cast<const HttpHeaderHost*>(header));
			case HTTP_HEADER_CONTENT_LENGTH:
				return new HttpHeaderContentLength(*static_cast<const HttpHeaderContentLength*>(header));
			default:
				return new HttpHeaderUnknown(*static_cast<const HttpHeaderUnknown*>(header));
		}
	}

	void HttpHeaderMap::deallocate_header_items()
	{
		HttpHeaderMap::iterator it;

		for (it = this->headers_.begin(); it != this->headers_.end(); it++)
			delete it->second;
	}

	HttpHeaderMap::iterator HttpHeaderMap::find(const std::string &key)
	{
		return this->headers_.find(key);
	}

	HttpHeaderMap::const_iterator HttpHeaderMap::find(const std::string &key) const
	{
		return this->headers_.find(key);
	}

	std::string HttpHeaderMap::header_type_to_string(HTTP_HEADER_TYPE type)
	{
		switch (type)
		{
			case HTTP_HEADER_ACCEPT:
				return "accept";
			case HTTP_HEADER_HOST:
				return "host";
			case HTTP_HEADER_CONTENT_LENGTH:
				return "content-length";
			default:
				return "unknown";
		}
	}

	std::pair<HttpHeaderMap::iterator,bool> HttpHeaderMap::insert(std::string header_name, HttpHeader *header)
	{
		std::pair<std::string, HttpHeader*>	header_pair;
		header_pair = std::make_pair(header_name, header);
		
		return this->headers_.insert(header_pair);
	}

	std::pair<HttpHeaderMap::iterator,bool> HttpHeaderMap::insert(HttpHeader *header)
	{
		std::string header_name = header_type_to_string(header->type);
		return this->insert(header_name, header);
	}

	void HttpHeaderMap::combine_value(HttpHeaderMap::iterator found_header, std::string header_value)
	{
		HttpHeaderListBased *list_based_header;

		list_based_header = dynamic_cast<HttpHeaderListBased *>(found_header->second);
		if (!list_based_header)
			throw std::runtime_error("Request: found more than one singleton header with the same name");
		list_based_header->list[0].append(", ");
		list_based_header->list[0].append(header_value);
	}

	HttpHeaderMap::iterator HttpHeaderMap::begin()
	{
		return this->headers_.begin();
	}

	HttpHeaderMap::const_iterator HttpHeaderMap::begin() const
	{
		return this->headers_.begin();
	}

	HttpHeaderMap::iterator HttpHeaderMap::end()
	{
		return this->headers_.end();
	}

	HttpHeaderMap::const_iterator HttpHeaderMap::end() const
	{
		return this->headers_.end();
	}

	const std::map<std::string, HttpHeader*> &HttpHeaderMap::get_headers() const
	{
		return this->headers_;
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
