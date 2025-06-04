#pragma once
namespace sorbengine::utility::path
{
	std::string& get_base_path();
	std::string& normalize_path(std::string& path);
	std::string& get_current_directory();
	std::string& get_pref_path(const std::string& org, const std::string& app);
	std::string get_absolute_asset_path(const std::string& path);
	std::string get_absolute_path(const std::string& path);
}
