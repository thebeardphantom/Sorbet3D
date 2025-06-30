#pragma once
#include <regex>
#include <string>
#include <cereal/details/helpers.hpp>

#if defined(__GNUG__)
#include <cxxabi.h>
#include <cstdlib>
#include <memory>
#endif

namespace sorbengine::utility
{
	inline std::string demangle(const char* name)
	{
#if defined(__GNUG__)
        int status = 0;
        std::unique_ptr<char, void(*)(void*)> result{
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free
        };
        return (status == 0) ? result.get() : name;
#else
		return name;
#endif
	}

	template <typename T>
	std::string get_name(const T& obj)
	{
		using decayed = std::decay_t<T>;

		std::string name;

		if constexpr (std::is_polymorphic_v<decayed>)
		{
			// get dynamic type
			name = demangle(typeid(obj).name());
		}
		else
		{
			// fallback: static type
			name = demangle(typeid(decayed).name());
		}

		// Strip prefixes like "class " or "struct "
		static const std::regex class_struct_regex(R"(\b(class|struct)\s+)");
		name = std::regex_replace(name, class_struct_regex, "");

		// Strip trailing " *" or MSVC pointer suffixes
		static const std::regex pointer_regex(R"(\s*\*\s*$)");
		name = std::regex_replace(name, pointer_regex, "");

		static const std::regex msvc_ptr_suffix(R"(\s*\*.*__ptr64\s*$)");
		name = std::regex_replace(name, msvc_ptr_suffix, "");

		const std::size_t pos = name.rfind("::");
		if (pos != std::string::npos)
		{
			name = name.substr(pos + 2);
		}

		return name;
	}

	template <typename T>
	std::string get_name(const std::unique_ptr<T>& obj)
	{
		return get_name(*obj);
	}

	template <typename Type>
	[[nodiscard]] static const cereal::NameValuePair<Type&>& get_obj_nvp(Type& obj)
	{
		// return dynamic_cast<cereal::NameValuePair<Type&>&>(nullptr);
		return cereal::make_nvp(get_name(obj).c_str(), obj);
	}
}
