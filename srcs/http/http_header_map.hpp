#pragma once

#include <map>
#include <string>
#include <utility>

#include "http_header.hpp"

namespace ws
{
	class HttpHeaderMap
	{
		public:
			typedef std::map<std::string, HttpHeader*>::iterator iterator;
			typedef std::map<std::string, HttpHeader*>::const_iterator const_iterator;

			HttpHeaderMap();
			~HttpHeaderMap();
			HttpHeaderMap(const HttpHeaderMap &src);
			HttpHeaderMap &operator=(const HttpHeaderMap &src);

			std::pair<iterator,bool> insert(std::string header_name, HttpHeader *header);
			std::pair<iterator,bool> insert(HttpHeader *header);
			void erase(iterator position);
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;
			iterator find(const std::string &key);
			const_iterator find(const std::string &key) const;
			iterator find(HTTP_HEADER_TYPE type);
			const_iterator find(HTTP_HEADER_TYPE type) const;
			void clear();
			const std::map<std::string, HttpHeader*> &get_headers() const;

		private:
			void deallocate_header_items();
			std::map<std::string, HttpHeader*> headers_;
	};

	bool operator==(const HttpHeaderMap &lhs, const HttpHeaderMap &rhs);
	bool operator!=(const HttpHeaderMap &lhs, const HttpHeaderMap &rhs);
}
