#pragma once
#include <string>
#include "engine_api.h"

namespace sorbengine::utility
{
	ENGINE_API std::string& get_base_path();
	ENGINE_API std::string normalize_path(std::string& path);
	ENGINE_API std::string& get_current_directory();
	ENGINE_API std::string get_pref_path(const std::string& org, const std::string& app);
	ENGINE_API std::string get_absolute_asset_path(const std::string& path);
	ENGINE_API std::string get_absolute_path(const std::string& path);
	ENGINE_API std::string get_file_ext(const std::string& path);
}
