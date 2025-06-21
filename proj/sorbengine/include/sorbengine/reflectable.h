#pragma once
#include <string>
#include <type_traits>
#include <entt/core/type_info.hpp>

namespace sorbengine::utility
{
	template <typename Type>
	[[nodiscard]] static const std::string& get_name()
	{
		static const std::string name = []
		{
			std::string_view full = entt::type_name<std::decay_t<Type>>().value();

			static const auto struct_prefix = std::string("struct ");
			static const auto class_prefix = std::string("class ");
			if (full.starts_with(struct_prefix))
			{
				full.remove_prefix(struct_prefix.length());
			}
			else if (full.starts_with(class_prefix))
			{
				full.remove_prefix(class_prefix.length());
			}

			static const auto namespace_separator = std::string("::");
			const std::size_t pos = full.rfind(namespace_separator);
			if (pos != std::string_view::npos)
			{
				full = full.substr(pos + namespace_separator.length());
			}

			return std::string(full);
		}();
		return name;
	}

	template <typename Type>
	[[nodiscard]] static const cereal::NameValuePair<Type&>& get_obj_nvp(Type& obj)
	{
		return cereal::make_nvp(get_name<Type>().c_str(), obj);
	}
}
