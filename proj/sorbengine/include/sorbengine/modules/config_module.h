#pragma once
#include <toml++/toml.hpp>
#include "engine_module.h"
#include "../engine_api.h"

namespace sorbengine::modules
{
	class config_module final : public engine_module
	{
	public:
		std::string get_name() override;
		SDL_AppResult init() override;
		ENGINE_API const toml::table& get_config();

		toml::node_view<const toml::node> operator[](const std::string& key) const noexcept;

	private:
		toml::table config_;
	};
}
