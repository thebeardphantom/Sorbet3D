#include "pch.h"
#include "string_utility.h"

namespace sorbengine
{
	std::string& string_utility::replace_all(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
		return str;
	}
}
