#include "pch.h"
#include "path_utility.h"
#include <SDL3/SDL_filesystem.h>
#include "string_utility.h"

namespace sorbet
{
	std::string& path_utility::get_base_path()
	{
		static std::string base_path = []
		{
			auto sdl_base_path = std::string(SDL_GetBasePath());
			return normalize_path(sdl_base_path);
		}();
		return base_path;
	}

	std::string& path_utility::normalize_path(std::string& path)
	{
		return string_utility::replace_all(path, "\\", "/");
	}

	std::string& path_utility::get_current_directory()
	{
		static std::string current_directory = []
		{
			auto sdl_current_directory = std::string(SDL_GetCurrentDirectory());
			return normalize_path(sdl_current_directory);
		}();
		return current_directory;
	}

	std::string& path_utility::get_pref_path(const std::string& org, const std::string& app)
	{
		const char* pref_path = SDL_GetPrefPath(org.c_str(), app.c_str());
		std::string pref_path_str = pref_path;
		return normalize_path(pref_path_str);
	}

	std::string path_utility::get_absolute_asset_path(const std::string& path)
	{
		return std::format("{}Assets/{}", get_base_path(), path);
	}

}