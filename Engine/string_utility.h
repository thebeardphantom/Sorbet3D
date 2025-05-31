#pragma once
namespace sorbet
{
	class string_utility
	{
	public:
		static std::string& replace_all(std::string& str, const std::string& from, const std::string& to);
	};
}
