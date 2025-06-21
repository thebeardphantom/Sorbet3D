#include "sorbengine/modules/config_module.h"
#include <toml++/toml.hpp>

#include "sorbengine/ecs_utility.h"
#include "sorbengine/path_utility.h"

namespace sorbengine::modules
{
	std::string config_module::get_name()
	{
		return "config_module";
	}

	SDL_AppResult config_module::init()
	{
		const auto path = utility::get_absolute_path("engine_default.toml");
		size_t bytes_read = 0;
		void* const file_data = SDL_LoadFile(path.c_str(), &bytes_read);
		const std::string file_str(static_cast<const char*>(file_data), bytes_read);
		// toml::parse requires a string
		config_ = toml::parse(file_str);
		SDL_free(file_data);
		return SDL_APP_CONTINUE;
	}

	const toml::table& config_module::get_config()
	{
		return config_;
	}

	toml::node_view<const toml::node> config_module::operator[](const std::string& key) const noexcept
	{
		return config_[key];
	}
}
