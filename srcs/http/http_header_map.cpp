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
			header = HttpHeader::alloc_new_header(it->second);
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
			header = HttpHeader::alloc_new_header(it->second);
			this->insert(it->first, header);
		}

		return *this;
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

	HttpHeaderMap::iterator HttpHeaderMap::find(HTTP_HEADER_TYPE type)
	{
		return this->headers_.find(HttpHeader::header_type_to_string(type));
	}
	
	HttpHeaderMap::const_iterator HttpHeaderMap::find(HTTP_HEADER_TYPE type) const
	{
		return this->headers_.find(HttpHeader::header_type_to_string(type));
	}

	std::pair<HttpHeaderMap::iterator,bool> HttpHeaderMap::insert(std::string header_name, HttpHeader *header)
	{
		std::pair<std::string, HttpHeader*>	header_pair;
		header_pair = std::make_pair(header_name, header);
		
		return this->headers_.insert(header_pair);
	}

	std::pair<HttpHeaderMap::iterator,bool> HttpHeaderMap::insert(HttpHeader *header)
	{
		std::string header_name = HttpHeader::header_type_to_string(header->type);
		return this->insert(header_name, header);
	}

	void HttpHeaderMap::erase(HttpHeaderMap::iterator position)
	{
		delete position->second;
		this->headers_.erase(position);
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

	void HttpHeaderMap::clear()
	{
		this->deallocate_header_items();
		this->headers_.clear();
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
