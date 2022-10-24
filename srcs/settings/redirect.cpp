#include "redirect.hpp"

namespace ws
{
	Redirect::Redirect() : code(0) {}

	bool operator==(const Redirect &lhs, const Redirect &rhs) {
		if (lhs.code == rhs.code && lhs.to == rhs.to) {
			return true;
		}
		return false;
	}
} // namespace ws
