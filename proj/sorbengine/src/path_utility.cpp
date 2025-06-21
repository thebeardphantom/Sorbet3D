#include "sorbengine/path_utility.h"
#include <filesystem>
#include <SDL3/SDL_filesystem.h>
#include "sorbengine/string_utility.h"

namespace sorbengine::utility
{
	std::string& get_base_path()
	{
		static std::string base_path = []
		{
			auto sdl_base_path = std::string(SDL_GetBasePath());
			return normalize_path(sdl_base_path);
		}();
		return base_path;
	}

	std::string normalize_path(std::string& path)
	{
		return replace_all(path, "\\", "/");
	}

	std::string& get_current_directory()
	{
		static std::string current_directory = []
		{
			auto sdl_current_directory = std::string(SDL_GetCurrentDirectory());
			return normalize_path(sdl_current_directory);
		}();
		return current_directory;
	}

	std::string get_pref_path(const std::string& org, const std::string& app)
	{
		const char* pref_path = SDL_GetPrefPath(org.c_str(), app.c_str());
		std::string pref_path_str = pref_path;
		return normalize_path(pref_path_str);
	}

	std::string get_absolute_asset_path(const std::string& path)
	{
		return std::filesystem::path(get_base_path()).concat("Assets/").concat(path).string();
	}

	std::string get_absolute_path(const std::string& path)
	{
		return get_base_path() + path;
	}

	std::string get_file_ext(const std::string& path)
	{
		const std::filesystem::path fs_path = path;
		return fs_path.extension().string();
	}
}
