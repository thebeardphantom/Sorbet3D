#pragma once
class path_utility
{
public:
	static std::string& get_base_path();
	static std::string& normalize_path(std::string& path);
	static std::string& get_current_directory();
	static std::string& get_pref_path(const std::string& org, const std::string& app);
	static std::string get_absolute_asset_path(const std::string& path);
};
